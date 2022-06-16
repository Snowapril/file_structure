// person.h
#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include "fixfld.h"
#include "length.h"
#include "delim.h"
using namespace std;

class Person 
{
protected: 
	// fields
	char PID[14];//주민번호
	char Name [20];//이름
	char Age [3];
	char City[10];
public:
	//operations
	Person ();
	static int InitBuffer (DelimFieldBuffer &);
	static int InitBuffer (LengthFieldBuffer &);
	static int InitBuffer (FixedFieldBuffer &);
	void Clear ();
	int Unpack (IOBuffer &);
	int Pack (IOBuffer &) const;
	void Print (ostream &, char * label = 0) const;
};
class Student : public Person {
private:
	char SID[12];//학번
	char Department[10];//학과명
	char Grade[2];//학년
public:
	Student();
	// 함수 추가할 것
};
#endif
