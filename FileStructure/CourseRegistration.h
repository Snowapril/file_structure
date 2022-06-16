#ifndef COURSE_REGISTRATION
#define COURSE_REGISTRATION

#include <iostream>
#include "fixfld.h"
#include "length.h"
#include "delim.h"

class CourseRegistration
{
public:
	friend std::ostream& operator<<(std::ostream& stream, const CourseRegistration& course);
	friend std::istream& operator>>(std::istream& stream, CourseRegistration& course);
public:
	CourseRegistration() = default;
	CourseRegistration(int courseIdentifier, int studentIdentifier, 
						int numCreditHours, float courseGrade);
	CourseRegistration(const CourseRegistration& other) = default;
	CourseRegistration& operator=(const CourseRegistration& other);

	void Print(std::ostream&, const char* label = 0) const;

	static int InitBuffer(DelimFieldBuffer&);
	static int InitBuffer(LengthFieldBuffer&);
	static int InitBuffer(FixedFieldBuffer&);

	void Clear(void);

	int Unpack(IOBuffer&);
	int Pack(IOBuffer&) const;

	std::string Key() const;

	int courseIdentifier;
	int studentIdentifier;
	int numCreditHours;
	float courseGrade;
};

#endif