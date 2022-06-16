
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include "iobuffer.h"
#include "person.h"
#include "tindbuff.h"
#include "textind.h"
#include "indfile.h"
#include "Student.h"
#include "delim.h"

/*
Using class Index, the index object should be stored into the RecordFile
*/
int main(int argc, char ** argv)
{
	char filename[20];
	int count, select, result, n, recaddr;
	Student student;
	Student *stable[10];
	DelimFieldBuffer Buffer;
	Student::InitBuffer(Buffer);

	TextIndex RecIndex(10);

	TextIndexedFile<Student> IndFile(Buffer, 12, 10);
	cout << "Testing TextIndex Buffer" << endl;
	TextIndexBuffer Buff(10, 3, 0, 0);
	Buff.Init(5);

	cout << "Enter the file name:" << flush;
	cin.getline(filename, 19);

	while (1)
	{
		cout << "\nSelect command 1: record input, 2. display, 3. write to file, 4. read from file, 5. make index file from data file, 6. use index file for reading data file, 7. Quit => ";
		cin >> select;

		switch (select) {
		case 1:
		{
			//to store a array of TextIndex objects by inputing
			cout << "the number of Person object records : ";
			cin >> count;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			for (int i = 0; i < count; i++)
			{
				cin >> student; //operator >>() should be implemented
				student.Print(cout);
				stable[i] = new Student(student);			// copy constructor should be implemented			
			}
			break;
		}
		case 2:
		{
			// to display the array of TextIndex objects into screen  
			for (int i = 0; i < count; i++)
			{
				cout << stable[i];
			}
			break;
		}
		case 3:
		{
			// to write the array of TextIndex objects into a file
			Buff.Print(cout);
			int result = IndFile.Create(filename, ios::out);
			cout << "Create file " << result << endl;
			if (!result)
			{
				cout << "Unable to Create file " << filename << endl;
				return -1;
			}

			for (int i = 0; i < count; i++)
			{
				recaddr = IndFile.Append(*stable[i]);
				const std::string key = stable[i]->Key();
				RecIndex.Insert(key.c_str(), recaddr);
				delete(stable[i]);
			}
			RecIndex.Print(std::cout);
			IndFile.Close();
			break;
		}
		case 4:
		{
			int result = IndFile.Open(filename, ios::in);
			if (result < 0)
			{
				cout << "Unable to read IndFile " << result << endl;
				return -1;
			}

			int recaddr = result;
			for (int i = 0; i < count; i++)
			{
				stable[i] = new Student;
				recaddr = IndFile.Read(*stable[i]);
				if (recaddr < 0) break;
				stable[i]->Unpack(Buffer);
				IndFile.Append(*stable[i]);
			}
			break;
		}
		case 5:
		{
			// make an indexed file from a Student file
			char recFilename[50] = { 0, };
			cout << endl << endl << "Input record file name : ";
			cin >> recFilename;

			int result;
			result = IndFile.Create("indfile", ios::out);
			if (!result)
			{
				cout << "Unable to create indfile " << result << endl;
				return 0;
			}

			Student student;
			int recaddr = -1, count = 0;
			DelimFieldBuffer Buffer;
			Student::InitBuffer(Buffer);
			BufferFile RecFile(Buffer);
			result = RecFile.Open(recFilename, ios::in);
			if (!result)
			{
				cout << "Unable to open file " << recFilename << endl;
				return 0;
			}

			cout << "Read Index from Record File \"" << recFilename << "\"" << endl;
			while (1)
			{
				recaddr = RecFile.Read();
				if (recaddr < 0)
					break;

				student.Unpack(Buffer);
				IndFile.Append(student);
				cout << recaddr << ' ' << student << endl;
			}
			cout << "Store index table into IndFile" << endl;
			IndFile.Close();
			cout << "Index saved at \"indfile.ind\"" << endl << endl;
			RecFile.Close();
			break;
		}
		case 6:
		{
			// read RecordFile by using index file.
			int count = 0;
			char indFileName[50] = { 0, };
			cout << endl << endl << "Input index file name : ";
			cin >> indFileName;

			int result = IndFile.Open(indFileName, ios::in);
			if (result < 0)
			{
				cout << "Unable to read IndFile \"" << indFileName << "\"" << endl;
				return -1;
			}

			std::string key;
			std::cout << "Enter the key : ";  cin >> key;
			cout << "select " << key << " at recaddr " << RecIndex.Search(key.c_str()) << endl;
			result = IndFile.Read(key.c_str(), student);
			cout << "read result: " << result << endl;
			if (result == -1) return false;
			cout << result;
			break;
		}
		default:
			// others
			exit(0);
			break;
		}
	}
	return 0;
}
