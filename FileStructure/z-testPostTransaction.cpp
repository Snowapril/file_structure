/*
교재: 367, 20번 programming project
Ledger 대신에 Student, Journal 대신에 course registration class를 정의한다.
*/

#include "recfile.h"
#include <fstream>
#include "ledger.h"
#include "length.h"
#include "mastrans.h"
#include <iomanip>

class Transcript
{
public:
	int Acct;
	char Title[30];
	double Balances[12];
	int Pack(IOBuffer& buffer) const;
	int Unpack(IOBuffer& buffer);
	ostream& Print(ostream&);
	ostream& PrintHeader(ostream&);
	Transcript();
	Transcript(int, const char*, double, double, double);
};

class TranscriptProcess : public MasterTransactionProcess<int>
	// Transcript processing of a Transcript file and a journal file
	// the item type is int to represent an account number
{
public:
	TranscriptProcess(int monthNumber); // constructor

	// Basic list processing methods
	int InitializeList(int ListNumber, const char* ListName) override;
	int InitializeOutput(const char* OutputListName) override;
	int NextItemInList(int ListNumber) override; //get next item from this list
	int Item(int ListNumber) override; // return current item from this list
	int ProcessItem(int ListNumber) override; // process the item in this list
	int FinishUp() override; // complete the processing

	// master/transaction methods
	int ProcessNewMaster() override;//  when new master read
	int ProcessCurrentMaster() override;// each transaction for a master
	int ProcessEndMaster() override;// after all transactions for a master
	int ProcessTransactionError() override;// no master for transaction

protected:
	// members
	int MonthNumber; // number of month to be processed
	LengthFieldBuffer Lbuffer, Jbuffer; // buffers for files
	RecordFile<Transcript> TranscriptFile; // list 1
	RecordFile<Journal> JournalFile;// list 2
	int AccountNumber[3]; // current item in each list
	Transcript Transcript; // current Transcript object
	Journal journal; // current journal object
	ofstream OutputList; // text output file for post method
	static int LowAcct;// lower than the lowest account number
	static int HighAcct;// higher than the highest account number

	int NextItemInTranscript();
	int NextItemInJournal();
};
int main(void)
{
	Transcript L1(101, "Checking Account #1", 1032.57, 1032.57, 1032.57);
	Journal J1(101, 1271, "02/02/86", "Auto expense", -78.70);
	LengthFieldBuffer Lbuffer;

	RecordFile<Transcript> TranscriptFile(Lbuffer);
	int result = TranscriptFile.Create("transcript.dat", ios::out);
	cout << "Create file " << result << endl;
	if (!result)
	{
		cout << "Unable to Create file " << "transcript.dat" << endl;
		return 0;
	}
	TranscriptFile.Write(L1);
	TranscriptFile.Close();

	LengthFieldBuffer Jbuffer;
	RecordFile<Journal> journalFile(Jbuffer);
	int flag = journalFile.Create("journal.dat", ios::out);
	cout << "Create file " << flag << endl;
	if (!flag)
	{
		cout << "Unable to Create file " << "journal.dat" << endl;
		return 0;
	}
	journalFile.Write(J1);
	journalFile.Close();

	int res;
	RecordFile<Transcript> lFile(Lbuffer);
	lFile.Open("transcript.dat", ios::in);
	Transcript Transcript;
	while (1)
	{
		res = lFile.Read(Transcript);
		if (res <= 0) break;
		cout << "Transcript" << endl;
		Transcript.Print(cout);
	}
	RecordFile<Journal> jFile(Jbuffer);
	jFile.Open("journal.dat", ios::in);
	Journal journal;
	while (1)
	{
		res = jFile.Read(journal);
		if (res <= 0) break;
		journal.PrintLine(cout);
	}
	cout << "Posting " << endl;

	TranscriptProcess Posting(0);
	/*
	Modify method TranscriptProcess::ProcessEndMaster so it updates the Transcript file
	with the new account balances for month
	*/

	Posting.PostTransactions("transcript.dat", "journal.dat", "transcript.txt");
	//Modify to show the file Transcript.txt
	char ch;
	fstream TranscriptJournal;
	TranscriptJournal.open("transcript.txt", ios::in);
	while (1) {
		TranscriptJournal >> ch;
		if (TranscriptJournal.fail()) break;
		cout << ch;
	}
	//Modify to show Transcript.Print();
	cout << "Showing the new account balances for the month" << endl;
	lFile.Open("transcript.dat", ios::in);
	//*
	while (1)
	{
		res = lFile.Read(Transcript);
		if (res <= 0) break;
		cout << "Transcript" << endl;
		Transcript.Print(cout);
	}

	system("pause");
	return 1;
}

int Transcript::Pack(IOBuffer& buffer) const
{
	buffer.Clear();
	buffer.Pack(&Acct, sizeof(Acct));
	buffer.Pack(&Title, -1);
	for (int i = 0; i < 12; i++)
		buffer.Pack(&Balances[i], sizeof(Balances[i]));
	return TRUE;
}

int Transcript::Unpack(IOBuffer& buffer)
{
	buffer.Unpack(&Acct, sizeof(Acct));
	buffer.Unpack(&Title, sizeof(Title));
	for (int i = 0; i < 12; i++)
		buffer.Unpack(&Balances[i], sizeof(Balances[i]));
	return TRUE;
}

Transcript::Transcript()
{
	Acct = 0;
	Title[0] = 0;
	for (int i = 0; i < 12; i++) Balances[i] = 0.0;
}

Transcript::Transcript(int acct, const char* title, double jan, double feb,
	double mar)
{
	Acct = acct;
	strcpy(Title, title);
	Balances[0] = jan;
	Balances[1] = feb;
	Balances[2] = mar;
	for (int i = 3; i < 12; i++) Balances[i] = 0.0;
}

ostream& Transcript::Print(ostream& stream)
{
	stream.setf(ios::right, ios::adjustfield);
	stream << Acct << '\t';
	stream.setf(ios::left, ios::adjustfield);
	stream << setw(20) << Title;
	stream.setf(ios::right, ios::adjustfield);
	stream.setf(ios::right | ios::fixed, ios::floatfield);
	stream.precision(2);
	for (int i = 0; i < 12; i++)
	{
		if (Balances[i] < 0.005 && i > 2)break;
		stream << '\t' << setw(8) << Balances[i];
	}
	stream << endl;
	return stream;
}

ostream& Transcript::PrintHeader(ostream& stream)
{// print the report header for this account
	stream << Acct << '\t' << Title << endl;
	return stream;
}

TranscriptProcess::TranscriptProcess(int monthNumber)
	: MasterTransactionProcess<int>(), TranscriptFile(Lbuffer), JournalFile(Jbuffer)
{
	MonthNumber = monthNumber;
}

int TranscriptProcess::InitializeList(int ListNumber, const char* ListName)
{
	AccountNumber[ListNumber] = LowAcct;
	switch (ListNumber)
	{
	case 1: // initialize ledger file
		return TranscriptFile.Open(ListName, ios::in);
	case 2: // initialize ledger file
		return JournalFile.Open(ListName, ios::in);
	}
	return FALSE;
}

int TranscriptProcess::InitializeOutput(const char* OutputListName)
{
	OutputList.open(OutputListName);
	return 1;
}

int TranscriptProcess::NextItemInList(int ListNumber)
//get next item from this list
{
	switch (ListNumber)
	{
	case 1: return NextItemInTranscript();
	case 2: return NextItemInJournal();
	}
	return FALSE;
}

int TranscriptProcess::Item(int ListNumber)
// return current item from this list
{
	return AccountNumber[ListNumber];
}

int TranscriptProcess::ProcessItem(int ListNumber)
// process the item in this list when it first appears
{
	switch (ListNumber)
	{
	case 1: // process new ledger object
		Transcript.PrintHeader(OutputList);
	case 2: // process journal file
		journal.PrintLine(OutputList);
	}
	return TRUE;
}

int TranscriptProcess::ProcessNewMaster()//  when new master read
{// first step in proceesing master record
 // print the header and setup last month's balance
	Transcript.PrintHeader(OutputList);
	Transcript.Balances[MonthNumber] = Transcript.Balances[MonthNumber - 1];
	return TRUE;
}

int TranscriptProcess::ProcessCurrentMaster()// each transaction for a master
{// add the transaction amount to the balance for this month	
	Transcript.Balances[MonthNumber] += journal.Amount;
	return TRUE;
}

int TranscriptProcess::ProcessEndMaster()// after all transactions for a master
{// print the balances line to output	
	PrintBalances(OutputList, Transcript.Balances[MonthNumber - 1], Transcript.Balances[MonthNumber]);
	return TRUE;
}

int TranscriptProcess::ProcessTransactionError()// no master for transaction
{// print error message to standard error
	cerr << "Error no account for journal" << endl;
	cerr << journal.Acct;
	journal.PrintLine(cerr);
	return TRUE;
}

int TranscriptProcess::FinishUp()
// complete the processing
{
	TranscriptFile.Close();
	JournalFile.Close();
	OutputList.close();
	return TRUE;
}

int TranscriptProcess::NextItemInTranscript()
{
	int res = TranscriptFile.Read(Transcript);
	if (res <= 0)
	{
		AccountNumber[1] = HighAcct;
		return FALSE;
	}
	if (Transcript.Acct <= AccountNumber[1])
	{
		cerr << "Transcript account out of order" << endl; exit(0);
	}
	AccountNumber[1] = Transcript.Acct;
	return TRUE;
}

int TranscriptProcess::NextItemInJournal()
{
	int res = JournalFile.Read(journal);
	if (res <= 0)
	{
		AccountNumber[2] = HighAcct;
		return FALSE;
	}
	if (journal.Acct < AccountNumber[2])
	{
		cerr << "Journal account out of order" << endl; exit(0);
	}
	AccountNumber[2] = journal.Acct;
	return TRUE;
}

int TranscriptProcess::LowAcct = -1;
int TranscriptProcess::HighAcct = 9999;