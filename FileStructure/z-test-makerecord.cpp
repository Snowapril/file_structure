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

//file에 write 한 후에 read하여 출력하는 프로그램 작성 실습 
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

	Student* R[10]; //=> 1장에서 만든 Student record로 변경한다.
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
		//TextIndex에 없으면 index.Insert()
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

	//IndexedFile, RecordFile에 write한 레코드를 읽는 코드를 추가한다
	const char* key = "139201";
	Student readRecord;
	cout << "Retrieve " << key << " at recaddr " << recordingIndex.Search(key) << endl;
	result = RecordedFile.Read(readRecord, recordingIndex.Search(key));
	cout << "read result : " << result;
	readRecord.Print(std::cout, "Key(139201) read result : ");

	IndexedFile.Close();
	RecordedFile.Close();

	//IndexedFile에 생성된 index를 저장
	TextIndexBuffer IndexBuffer(12, 10);
	BufferFile IndexFile(IndexBuffer);
	IndexBuffer.Pack(recordingIndex);
	IndexFile.Create("recindex.dat", ios::out);
	IndexFile.Write();
	IndexFile.Close();

	IndexedFile.Open("record.ind", ios::in); //다시 open
	RecordedFile.Open("recording.dat", ios::in);//다시 open
	//const char *keyname = "DG18807"; 화면에서 입력받아서 반복 실행하게 처리
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
	//입력된 key에 대한 index를 search한 후에 해당 레코드를 출력하는 프로그램 - makeind-2.cpp를 참조하여
	// 본 프로그램을 수정하여 완성
	system("pause");
	return 1;
}


