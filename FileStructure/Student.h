#ifndef STUDENT
#define STUDENT

#include <string>
#include <iostream>

#include "fixfld.h"
#include "length.h"
#include "delim.h"

class Student
{
	friend std::ostream& operator<<(std::ostream& stream, const Student& student);
	friend std::istream& operator>>(std::istream& stream, Student& student);
public:
	Student() = default;
	Student(int identifier, std::string name, std::string address,
			std::string firstEnrollDate, int numCreditHours);
	Student(const Student& other) = default;
	Student& operator=(const Student& other);

	inline void incrNumCreditHours(void);

	void Print(std::ostream&, const char* label = 0) const;

	static int InitBuffer(DelimFieldBuffer&);
	static int InitBuffer(LengthFieldBuffer&);
	static int InitBuffer(FixedFieldBuffer&);

	void Clear(void);

	int Unpack(IOBuffer&);
	int Pack(IOBuffer&) const;

	std::string Key() const;

	int identifier;
	std::string name;
	std::string address;
	std::string firstEnrollDate;
	int numCreditHours;
};

#endif