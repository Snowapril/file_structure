//makerec.cc
#include "buffile.h"
#include "delim.h"
#include <iostream>
#include "Student.h"
#include "recfile.h"
#include <fstream>
#include <iomanip>
#include "iobuffer.h"
#include "textind.h"
#include "tindbuff.h"

//file�� write �� �Ŀ� read�Ͽ� ����ϴ� ���α׷� �ۼ� �ǽ� 
// create two files with the recordings of Figure 7.2
//    the file "record.dat" is created using class BufferFile
//    the file "record2.dat" is created using class RecordFile<Recording>
using namespace std;
int main(void)
{
	int recaddr = 0;
	int result;

	TextIndex recordingIndex(10);

	DelimFieldBuffer::SetDefaultDelim('|');
	DelimFieldBuffer Buffer;
	Student::InitBuffer(Buffer);

	BufferFile IndexedFile(Buffer);

	const char* myfile = "record.ind";
	result = IndexedFile.Create(myfile, ios::out);
	cout << "main()::create file " << result << endl;
	if (!result)
	{
		cout << "main()::Unable to create file " << myfile << endl;
		result = IndexedFile.Open(myfile, ios::out);
		cout << "main()::open file " << result << endl;
		if (!result)
		{
			cout << "main()::Unable to open file " << myfile << endl;
			system("pause");

			return 0;
		}
	}
	
	RecordFile<Student> RecordedFile(Buffer);
	RecordedFile.Create("recording.dat", ios::out);

	Student* R[10]; //=> 1�忡�� ���� Student record�� �����Ѵ�.
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
		//TextIndex�� ������ index.Insert()
		cout << "Recordin2 R[" << i << "] at recaddr " << recaddr << endl;
		const std::string key = R[i]->Key();
		if (recordingIndex.Search(key.c_str()) == -1)
		{
			recordingIndex.Insert(key.c_str(), recaddr);
		}
		recaddr = RecordedFile.Write(*R[i]);
		delete R[i];
	}
	recordingIndex.Print(std::cout);

	//IndexedFile, RecordFile�� write�� ���ڵ带 �д� �ڵ带 �߰��Ѵ�
	const char* key = "139201";
	Student readRecord;
	cout << "Retrieve " << key << " at recaddr " << recordingIndex.Search(key) << endl;
	result = RecordedFile.Read(readRecord, recordingIndex.Search(key));
	cout << "read result : " << result;
	readRecord.Print(std::cout, "Key(139201) read result : ");

	IndexedFile.Close();
	RecordedFile.Close();

	//IndexedFile�� ������ index�� ����
	TextIndexBuffer IndexBuffer(12, 10);
	BufferFile IndexFile(IndexBuffer);
	IndexBuffer.Pack(recordingIndex);
	IndexFile.Create("recindex.dat", ios::out);
	IndexFile.Write();
	IndexFile.Close();

	IndexedFile.Open("record.ind", ios::in); //�ٽ� open
	RecordedFile.Open("recording.dat", ios::in);//�ٽ� open
	//const char *keyname = "DG18807"; ȭ�鿡�� �Է¹޾Ƽ� �ݺ� �����ϰ� ó��
	while (true)
	{
		readRecord.Clear();
		std::string key;
		std::cout << "Enter the key for searching : ";

		if (!(std::cin >> key)) break;

		int index = recordingIndex.Search(key.c_str());
		cout << "Retrieve " << key << " at recaddr " << recordingIndex.Search(key.c_str()) << endl;
		result = RecordedFile.Read(readRecord, recordingIndex.Search(key.c_str()));
		cout << "read result: " << result << endl;
		if (result == -1) return FALSE;
		readRecord.Print(std::cout, "Retrieved data : ");
	}
	//�Էµ� key�� ���� index�� search�� �Ŀ� �ش� ���ڵ带 ����ϴ� ���α׷� - makeind-2.cpp�� �����Ͽ�
	// �� ���α׷��� �����Ͽ� �ϼ�
	system("pause");
	return 1;
}


