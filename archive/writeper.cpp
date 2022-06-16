// D.7 writestr.cpp - 가변길이 레코드 + 가변길이 필드
// write a stream of persons, using fstream.h 
#include <iostream>
#include <fstream>
#include <string.h>
#include "person.h"
using namespace std;

//Fig 4.7 write a variable-length, delimited buffer to a file
const int MaxBufferSize = 200;
int WritePerson (ostream & stream, Person & p)
{	char buffer [MaxBufferSize];
	strcpy(buffer, p.LastName); strcat(buffer,"|");
	strcat(buffer, p.FirstName); strcat(buffer,"|");
	strcat(buffer, p.Address);  strcat(buffer,"|");
	strcat(buffer, p.City);  strcat(buffer,"|");
	strcat(buffer, p.State);  strcat(buffer,"|");
	strcat(buffer, p.ZipCode);  strcat(buffer,"|");
	short length=strlen(buffer); 
	stream.write ((char *)&length, sizeof(length)); // write length
	stream.write (&buffer[0], length);
	return 1;
}
istream& operator >> (istream& stream, Person& p)
{ // read fields from input
	cout << "Enter last name, or <cr> to end: " << flush;
	stream.getline(p.LastName, 30);
	if (strlen(p.LastName) == 0) return stream;
	cout << "Enter first name: " << flush; stream.getline(p.FirstName, 30);
	cout << "Enter address: " << flush; stream.getline(p.Address, 30);
	cout << "Enter city: " << flush; stream.getline(p.City, 30);
	cout << "Enter state: " << flush; stream.getline(p.State, 15);
	cout << "Enter zip code: " << flush; stream.getline(p.ZipCode, 10);
	return stream;
}
int main (void){
	char filename [20];
	Person p;
	cout << "Enter the file name:"<<flush;
	cin.getline(filename, 19);
	ofstream stream (filename, ios::out);
	if (stream.fail()) {
		cout << "File open failed!" <<endl;
		return 0;
	}
	while (1) {
		// read fields of person
		cin >> p;
		if (strlen(p.LastName)==0) break;
		// write person to output stream
		WritePerson(stream,p);
	}
	system("pause");
	return 1;
}

