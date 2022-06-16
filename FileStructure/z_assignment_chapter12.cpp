/* 12장 extendible hashing 프로그래밍 과제 
1. page 563: Programming Project: 20,21,22
2. an existing student record file: RecordFile<Student> StudnetDataFile(Buffer);
  - data file을 먼저 만들고 file을 close한다. 다음에 data file을 읽어서 hashed index를 만든다.
  - 	resultIndex = Dir.Create("studenthash");
  최소 6개 이상 레코드 입력
3. an existing course registration record file: RecordFile<CourseRegistration> CourseRegistrationDataFile(Buffer);
  - 	resultIndex = Dir.Create("coursehash");
  - data file을 먼저 만들고 file을 close한다. 다음에 data file을 읽어서 hashed index를 만든다.
  - 각 Sid에 대하여 매칭되는 수강 과목이 4개 이상되도록 입력
4. 22번 문제는 SId를 입력받아 다음과 같이 출력한다.(입력받은 SId와 교과목 리스트에 대하여 찾기를 반복한다)
    s123 홍길동  컴퓨터 3
	      DB  3.0
		  OS  4.0
	s124 홍길순  전자   4
	      File Structure 3.5
		  Data Structure 4.5
5. course list는 DB, OS, File Structure, Data Structure, CPP, C Programming, Network, Web Programming, AI, Big Data
6. 수강 취소는 CourseRegistration index에서 remove key를 입력받아 사용하고 data file에서 삭제 마크 '0000'로 처리하고 
    Read()에서 삭제 마크로 표시된 레코드는 출력하지 않는다.
*/
#include "direct.h"
#include "bucket.h"
#include "hash.h"
#include <conio.h>
#include "recording.h"
#include <fstream>
#include <iomanip>
#include <string>
#include "iobuffer.h"
#include "fixfld.h"
#include "length.h"
#include "buffile.h"
#include "recfile.h"
#include "indbuff.h"
#include <iostream>
#include <string.h>
#include <strstream>
#pragma warning(disable : 4996)
using namespace std;

class Student {
public:
	Student() = default;
	~Student() = default;
	Student(const char* sid, const char* sname, const char* dept, const char* syear)
	{
		strcpy(SId, sid); strcpy(SName, sname);
		strcpy(Dept, dept); strcpy(SYear, syear);
		SId[strlen(sid)] = '\0';
		SName[strlen(sname)] = '\0';
		Dept[strlen(dept)] = '\0';
		SYear[strlen(syear)] = '\0';
	}
	const char* Key() const { return SId; }
	int Pack(IOBuffer& Buffer) const
	{// return TRUE if all succeed, FALSE o/w
		int numBytes;
		Buffer.Clear();
		numBytes = Buffer.Pack(SId);
		if (numBytes == -1) return FALSE;
		numBytes = Buffer.Pack(SName);
		if (numBytes == -1) return FALSE;
		numBytes = Buffer.Pack(Dept);
		if (numBytes == -1) return FALSE;
		numBytes = Buffer.Pack(SYear);
		if (numBytes == -1) return FALSE;
		return TRUE;
	}

	int Unpack(IOBuffer& Buffer)
	{// unpack with maximum size, and add null termination to strings
		int numBytes;
		numBytes = Buffer.Unpack(SId, 4);
		if (numBytes == -1) return FALSE;
		SId[numBytes] = 0;
		numBytes = Buffer.Unpack(SName, 13);
		if (numBytes == -1) return FALSE;
		SName[numBytes] = 0;
		numBytes = Buffer.Unpack(Dept, 9);
		if (numBytes == -1) return FALSE;
		Dept[numBytes] = 0;
		numBytes = Buffer.Unpack(SYear, 4);
		if (numBytes == -1) return FALSE;
		return TRUE;
	}

	void Print(ostream& stream) const
	{
		stream << SId << '|' << SName << '|' << Dept << '|'
			<< SYear;
	}
	char SId[5]; char SName[14]; char Dept[10];	char SYear[5];
};
class CourseRegistration {
public:
	CourseRegistration() = default;
	~CourseRegistration() = default;
	CourseRegistration(const char* sid, const char* coursename, const char* grade)
	{
		strcpy(SId, sid); strcpy(CourseName, coursename); strcpy(Grade, grade);
	}
	const char* Key() const
	{
		ostrstream key;
		key << SId << CourseName << ends;
		return key.str();
	}
	int Pack(IOBuffer& Buffer) const
	{// return TRUE if all succeed, FALSE o/w
		int numBytes;
		Buffer.Clear();
		numBytes = Buffer.Pack(SId);
		if (numBytes == -1) return FALSE;
		numBytes = Buffer.Pack(CourseName);
		if (numBytes == -1) return FALSE;
		numBytes = Buffer.Pack(Grade);
		if (numBytes == -1) return FALSE;
		return TRUE;
	}

	int Unpack(IOBuffer& Buffer)
	{// unpack with maximum size, and add null termination to strings
		int numBytes;
		numBytes = Buffer.Unpack(SId, 4);
		if (numBytes == -1) return FALSE;
		SId[numBytes] = 0;
		numBytes = Buffer.Unpack(CourseName, 23);
		if (numBytes == -1) return FALSE;
		CourseName[numBytes] = 0;
		numBytes = Buffer.Unpack(Grade, 3);
		if (numBytes == -1) return FALSE;
		Grade[numBytes] = 0;
		return TRUE;
	}

	void Print(ostream& stream) const
	{
		stream << SId << '|' << SId << '|' << CourseName << '|'
			<< Grade;
	}
	char SId[5]; char CourseName[24]; char Grade[4];
};

int main(int argc, char** argv)
{
	const char* CourseNames[] = {
		"DB", "OS", "File Structure", "Data Structure", "CPP", "C Programming", "Network", "Web Programming", "AI", "Big Data"
	};
	Student *StudentRecords[10];
	StudentRecords[0] = new Student("0000", "John", "CS", "2016");
	StudentRecords[1] = new Student("0001", "Silly", "CS", "2016");
	StudentRecords[2] = new Student("0002", "Diana", "CS", "2010");
	StudentRecords[3] = new Student("0003", "Kong", "CS", "2016");
	StudentRecords[4] = new Student("0004", "Pong", "CS", "2011");
	StudentRecords[5] = new Student("0005", "Chang", "CS", "2012");
	StudentRecords[6] = new Student("0006", "Please", "CS", "2016");
	StudentRecords[7] = new Student("0007", "Jongang", "CS", "2016");
	StudentRecords[8] = new Student("0008", "Tired", "CS", "2018");
	StudentRecords[9] = new Student("0009", "Sleep", "CS", "2017");

	CourseRegistration* CourseRecords[10];
	CourseRecords[0] = new CourseRegistration("0000", "DB", "100");
	CourseRecords[1] = new CourseRegistration("0000", "OS", "90");
	CourseRecords[2] = new CourseRegistration("0000", "File Structure", "100");
	CourseRecords[3] = new CourseRegistration("0000", "Data Structure", "100");
	CourseRecords[4] = new CourseRegistration("0000", "CPP", "100");
	CourseRecords[5] = new CourseRegistration("0000", "C Programming", "100");
	CourseRecords[6] = new CourseRegistration("0004", "Network", "100");
	CourseRecords[7] = new CourseRegistration("0002", "Web Programming", "100");
	CourseRecords[8] = new CourseRegistration("0001", "AI", "100");
	CourseRecords[9] = new CourseRegistration("0003", "Big Data", "100");

	int count, select, resultFile, resultIndex;
	char removeKey[32], searchKey[15];
	Directory StudentDir(4);
	resultIndex = StudentDir.Create("studenthash");
	FixedFieldBuffer StudentBuffer(10);
	StudentBuffer.AddField(5);
	StudentBuffer.AddField(14);
	StudentBuffer.AddField(10);
	StudentBuffer.AddField(5);
	// Buffer.InitFields(); // Add each field size of record to buffer (can change if you need)
	RecordFile<Student> StudentDataFile(StudentBuffer);

	resultFile = StudentDataFile.Create("studentdatafile.dat", ios::in | ios::out);
	cout << "Create file " << resultFile << endl;
	if (!resultFile)
	{
		cout << "Unable to Create file : studentdatafile.dat" << endl;
		return 0;
	}

	Directory CourseDir(4);
	resultIndex = CourseDir.Create("coursehash");
	FixedFieldBuffer CourseBuffer(10);
	CourseBuffer.AddField(5);
	CourseBuffer.AddField(24);
	CourseBuffer.AddField(4);
	RecordFile<CourseRegistration> CourseDataFile(CourseBuffer);

	resultFile = CourseDataFile.Create("coursedatafile.dat", ios::in | ios::out);
	cout << "Create file " << resultFile << endl;
	if (!resultFile)
	{
		cout << "Unable to Create file : coursedatafile.dat" << endl;
		return 0;
	}
	Student student;
	CourseRegistration course;
	int recaddr;
	ostrstream key;
	char SId[15];
	char SCourseName[24];

	while (1)
	{
		cout << "\nSelect command 1: insert student record set, 2: insert course registration record set, 3. create a hash file from an student record file," <<
			"4. create a hash file from an course registration record file, 5. print all course grades that match the input Sid, 6. delete a course registration record with input SId and course name, "
			<< "7. print the course registration record set, 8. Quit => ";
		cin >> select;

		switch (select) {
		case 1://input data record set
			for (int i = 0; i < 10; i++)
			{
				StudentRecords[i]->Print(cout);
				StudentRecords[i]->Pack(StudentBuffer);
				recaddr = StudentDataFile.Write(*StudentRecords[i]);
				cout << "DataFile R[" << i << "] at recaddr " << recaddr << endl;
				const char* keys = StudentRecords[i]->Key();
				cout << "Inserting with key = " << keys << endl;
				//1 block = 4 records
				delete StudentRecords[i];
			}
			StudentDataFile.Close();
			break;
		case 2://2: insert course registration record set
			for (int i = 0; i < 10; i++)
			{
				CourseRecords[i]->Print(cout);
				CourseRecords[i]->Pack(CourseBuffer);
				recaddr = CourseDataFile.Write(*CourseRecords[i]);
				cout << "DataFile R[" << i << "] at recaddr " << recaddr << endl;
				const char* keys = CourseRecords[i]->Key();
				cout << "Inserting with key = " << keys << endl;
				//1 block = 4 records
				delete CourseRecords[i];
			}
			CourseDataFile.Close();
			break;
		case 3:
			resultFile = StudentDataFile.Open("studentdatafile.dat", ios::in);
			if (!resultFile)
			{
				cout << "Unable to open data file" << endl;
				return 0;
			}
			for (int i = 0; i < 10; i++)
			{
				recaddr = StudentDataFile.Read(student);
				const char* keys = student.Key();
				cout << (void*)Hash(keys);//(void*) casting은 bit string
				cout << endl << endl << keys << "  " << (void*)MakeAddress(keys, 16) << endl << endl;
				resultIndex = StudentDir.Insert(keys, recaddr);
				if (resultIndex == 0) cout << "Insert for " << keys[i] << " failed" << endl;
				StudentDir.Print(cout);
			}
			StudentDir.Close();
			StudentDataFile.Close();
			break;
		case 4:
			resultFile = CourseDataFile.Open("coursedatafile.dat", ios::in);
			if (!resultFile)
			{
				cout << "Unable to open data file" << endl;
				return 0;
			}
			for (int i = 0; i < 10; i++)
			{
				recaddr = CourseDataFile.Read(course);
				const char* keys = course.Key();
				cout << (void*)Hash(keys);//(void*) casting은 bit string
				cout << endl << endl << keys << "  " << (void*)MakeAddress(keys, 16) << endl << endl;
				resultIndex = CourseDir.Insert(keys, recaddr);
				if (resultIndex == 0) cout << "Insert for " << keys[i] << " failed" << endl;
				CourseDir.Print(cout);
			}
			CourseDir.Close();
			CourseDataFile.Close();
			cout << endl;
			break;
		case 5://5. print all course grades that match the input Sid
			resultFile = CourseDataFile.Open("coursedatafile.dat", ios::in | ios::out);
			if (!resultFile)
			{
				cout << "Unable to open data file" << endl;
				return 0;
			}
			resultIndex = CourseDir.Open("coursehash");
			if (resultIndex == 0)
			{
				cout << "Please create coursehash.dir and coursehash.bkt" << endl;
				return 0;
			}
			cout << "Input SId what search : ";
			cin >> SId;
			
			for (const char* names : CourseNames)
			{
				ostrstream merger;
				merger << SId << names << ends;
				strcpy(searchKey, merger.str());
				(void*)MakeAddress(searchKey, 16);

				recaddr = CourseDir.Search(searchKey);
				if (recaddr == -1)
				{
					continue;
				}
				cout << endl << endl << searchKey << "  " << (void*)MakeAddress(searchKey, 16) << endl;
				cout << "RecAddr : " << recaddr << endl;

				CourseDataFile.Read(course, recaddr);
				course.Print(cout);
			}

			CourseDataFile.Close();
			CourseDir.Close();
			break;
		case 6://6. delete a course registration record with input SId and course name
			resultFile = CourseDataFile.Open("coursedatafile.dat", ios::in | ios::out);
			if (!resultFile)
			{
				cout << "Unable to open data file" << endl;
				return 0;
			}
			resultIndex = CourseDir.Open("coursehash");
			if (resultIndex == 0)
			{
				cout << "Please create coursehash.dir and coursehash.bkt" << endl;
				return 0;
			}
			cout << "Input SId what remove : ";
			cin >> SId;
			cout << "Input Course Name what remove : ";
			cin >> SCourseName;

			key.clear();
			key << SId << SCourseName << ends;
			strcpy(removeKey, key.str());

			cout << endl << endl << removeKey << "  " << (void*)MakeAddress(removeKey, 16) << endl << endl;
			recaddr = CourseDir.Search(removeKey);
			CourseDataFile.Read(course, recaddr);
			course.SId[0] = '\0';
			course.CourseName[0] = '\0';
			course.Grade[0] = '\0';
			CourseDataFile.Write(course, recaddr);//update mark on key field
			resultIndex = CourseDir.Remove(removeKey);
			if (resultIndex == 0) cout << "Remove for " << removeKey << " failed" << endl;
			CourseDir.Print(cout);
			CourseDataFile.Close();
			CourseDir.Close();
			break;
		case 7: // 5. print the course registration record set
			resultFile = CourseDataFile.Open("coursedatafile.dat", ios::in);
			if (!resultFile)
			{
				cout << "Unable to open data file" << endl;
				return 0;
			}
			for (int i = 0; i < 10; i++)
			{
				CourseDataFile.Read(course, -1);
				const char* keys = course.Key();
				if (strcmp(keys, "") != 0)
					course.Print(cout);
			}
			CourseDataFile.Close();
			break;
		case 8:
		default:
			exit(0);
			break;
		}
	}
	system("pause");
	return 1;
}