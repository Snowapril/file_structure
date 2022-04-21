//Student.cc
#include "person.h"

Student::Student() { Clear(); }

void Student::Clear()
{
	// set each field to an empty string
//완성해야 함
}

int Student::Pack(IOBuffer & Buffer) const
{// pack the fields into a FixedFieldBuffer, 
 // return TRUE if all succeed, FALSE o/w
	int numBytes;
	Buffer.Clear();
// 완성해야 함

	return TRUE;
}

int Student::Unpack(IOBuffer & Buffer)
{
	Clear();
	int numBytes;
	//완성해야 함
	return TRUE;
}


void Student::Print(ostream & stream, char * label = 0) const
{
	//수정 필요
	if (label == 0) stream << "Student:";
	else stream << label;
	stream << "\n\t SID '" << SID << "'\n";

}

istream& operator>>(istream &, Student &)
{
	// 구현 필요
}
ostream& operator<<(ostream &, Student &)
{
	// 구현 필요
}