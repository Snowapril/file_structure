// writedel.cpp => readdel.cpp
// write a stream of persons, using fstream.h 
/*
istream::getline
istream& getline(char*s, streamsize n);//�Է� stream ���� ���� ���ڸ� �о�鿩 s�� �����Ѵ�. delimiter�� '\n'�� ����Ѵ�.
istream& getline(char*s, streamsize n, char delim);//delimiter�� ��Ÿ�� ������ �а� �ȴ�. delimiter�� delim ���� ���޵ȴ�.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include "Student.h"
#include "CourseRegistration.h"
using namespace std;

// Figure 4.1 => 2�� �ҽ��ڵ�� �ǽ�
std::ostream& operator<<(std::ostream& stream, const CourseRegistration& course)
{
	stream << course.courseIdentifier	<< '|'
		   << course.studentIdentifier	<< '|'
		   << course.numCreditHours		<< '|'
		   << course.courseGrade		<< '|';
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Student& student)
{
	stream << student.identifier		<< '|'
		   << student.name				<< '|'
		   << student.address			<< '|'
		   << student.firstEnrollDate	<< '|'
		   << student.numCreditHours	<< '|';
	return stream;
}

istream& operator >> (istream& stream, CourseRegistration& course)
{ // read fields from input
	char temp[32];
	cout << "Enter course identifier, or <cr> to end: " << flush;
	stream.getline(temp, 32);
	if (strlen(temp) == 0)
	{
		course.courseIdentifier = -1;
		return stream;
	}
	course.courseIdentifier = std::stoi(temp);

	cout << "Enter student identifier: " << flush; stream.getline(temp, 32);
	course.studentIdentifier = std::stoi(temp);

	cout << "Enter number of credit hours: " << flush; stream.getline(temp, 32);
	course.numCreditHours = std::stoi(temp);

	cout << "Enter course grade: " << flush; stream.getline(temp, 32);
	course.courseGrade = std::stof(temp);

	return stream;
}

istream& operator >> (istream& stream, Student& student)
{ // read fields from input
	char temp[32];
	cout << "Enter student identifier, or <cr> to end: " << flush;
	stream.getline(temp, 32);
	if (strlen(temp) == 0)
	{
		student.identifier = -1;
		return stream;
	}
	student.identifier = std::stoi(temp);

	cout << "Enter name: " << flush; stream.getline(temp, 32);
	student.name = std::string(temp);

	cout << "Enter address: " << flush; stream.getline(temp, 32);
	student.address = std::string(temp);

	cout << "Enter first enroll date: " << flush; stream.getline(temp, 32);
	student.firstEnrollDate = std::string(temp);

	cout << "Enter number of credit hours: " << flush; stream.getline(temp, 32);
	student.numCreditHours = std::stoi(temp);
		
	return stream;
}

int main (void){
	char filename [20];
	cout << "Enter the file name:"<<flush;
	cin.getline(filename, 19);
	ofstream file (filename, ios::out);
	if (file.fail()) {
		cout << "File open failed!" <<endl;
		return 0;
	}

	CourseRegistration course;
	cout << "Start entering for course registrations\n" << flush;
	while (1) {
		// read fields of course
		cin >> course;
		if (course.courseIdentifier == -1) break;
		// write course to file
		file << course;
	}

	Student student;
	cout << "Start entering for students\n" << flush;
	while (1) {
		// read fields of course
		cin >> student;
		if (student.identifier == -1) break;
		// write course to file
		file << student;
	}

	file.close();

	cout << "Entering information finished\n" << flush;
	system("pause");
	return 1;
}
