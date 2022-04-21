//testind.cc - TextindexedFile 사용 실습
#include "textind.h"
#include "delim.h"
#include "buffile.h"
#include "Student.h"
#include "indfile.h"
#include <iostream>

int main (void)
{// first argument is the file name for the data file
	int result;

	TextIndex RecIndex (11);
	DelimFieldBuffer Buffer; // create a buffer
	TextIndexedFile<Student> IndFile (Buffer, 12, 11);
	//TextIndexedFile<RecType>::TextIndexedFile(IOBuffer & buffer, int keySize, int maxKeys)
	result = IndFile . Create ("indfile", ios::out);
	if (!result) 
	{
		cout<<"Unable to open indfile "<<result<<endl;
		return 0;
	}	
	int recaddr;
	//write records
	Student* R[10], foundRecord;
	R[0] = new Student(2312, "LON", "Romeo and Juliet", "2022/03/01", 10);
	R[1] = new Student(2626, "RCA", "Quartet in C Sharp Minor", "2022/03/01", 10);
	R[2] = new Student(23699, "WAR", "Touchstone", "2022/03/01", 10);
	R[3] = new Student(3795, "ANG", "Symphony No. 9", "2022/03/01", 10);
	R[4] = new Student(38358, "COL", "Nebraska", "2022/03/01", 10);
	R[5] = new Student(18807, "DG", "Symphony No. 9", "2022/03/01", 10);
	R[6] = new Student(75016, "MER", "Coq d'or Suite", "2022/03/01", 10);
	R[7] = new Student(31809, "COL", "Symphony No. 9", "2022/03/01", 10);
	R[8] = new Student(139201, "DG", "Violin Concerto", "2022/03/01", 10);
	R[9] = new Student(245, "FF", "Good News", "2022/03/01", 10);
	for (int i = 0; i < 10; i++)
	{
		recaddr = IndFile.Append(*R[i]);
		cout << "IndFile R[" << i << "] at recaddr " << recaddr << endl;
	}

	Student student;
	cout << endl << "Enter the last Student record" << endl;
	cin >> student;
	recaddr = IndFile.Append(student);
	cout << "IndFile R[" << 10 << "] at recaddr " << recaddr << endl;
	IndFile.Close();

	DelimFieldBuffer Buffer2; // create a buffer
	TextIndexedFile<Student> IndFile2(Buffer2, 12, 11);

	IndFile2.Open("indfile", ios::in);

	RecordFile<Student> RecFile(Buffer2);
	result = RecFile.Open("indfile.dat", ios::in);
	if (!result)
	{
		cout << "Unable to open file " << "indfile.dat" << endl;
		return 0;
	}

	Student fileRec, indRec;
	while (1) // loop until the read fails
	{
		recaddr = RecFile.Read(fileRec); // read next record
		if (recaddr < 0) break;
		cout << "\nLooking for record with key:"
			<< fileRec.Key() << endl;
		const std::string key = fileRec.Key();
		result = IndFile2.Read(key.c_str(), indRec);
		if (result < 0) cout << "record not found";//need to make code for making index file
		else indRec.Print(cout);
	}

	std::string searchKey;
	std::cout << "Enter the key for searching : "; std::cin >> searchKey;
	IndFile.Read(searchKey.c_str(), foundRecord);
	foundRecord.Print(cout);
	IndFile.Close();
	system("pause");
	return 1;
}

