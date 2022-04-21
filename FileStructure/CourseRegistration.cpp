#include "CourseRegistration.h"
#include <string>

std::ostream& operator<<(std::ostream& stream, const CourseRegistration& course)
{
	course.Print(stream);
	return stream;
}

std::istream& operator>>(std::istream& stream, CourseRegistration& course)
{
	std::cout << "Enter the course identifier: "; stream >> course.courseIdentifier;
	std::cout << "Enter the student identifier: "; stream >> course.studentIdentifier;
	std::cout << "Enter the num credit hours: "; stream >> course.numCreditHours;
	std::cout << "Enter the course grade: "; stream >> course.courseGrade;
	return stream;
}

CourseRegistration::CourseRegistration(int courseIdentifier, int studentIdentifier,
	int numCreditHours, float courseGrade)
		:courseIdentifier(courseIdentifier), studentIdentifier(studentIdentifier),
		numCreditHours(numCreditHours), courseGrade(courseGrade)
{
	// Do nothing
}

CourseRegistration& CourseRegistration::operator=(const CourseRegistration& other)
{
	if (this == &other)
	{
		courseIdentifier = other.courseIdentifier;
		studentIdentifier = other.studentIdentifier;
		numCreditHours = other.numCreditHours;
		courseGrade = other.courseGrade;
	}
	return *this;
}

void CourseRegistration::Print(std::ostream& stream, const char* label) const
{
	if (label == 0) stream << "CourseRegistration:";
	else stream << label;
	stream << "	Course ID		'"			<< courseIdentifier		<< "'\n"
		<< "\t		Student ID		'"		<< studentIdentifier	<< "'\n"
		<< "\t		Num Credit Hours	'"	<< numCreditHours		<< "'\n"
		<< "\t\t	Course Grade		'"	<< courseGrade			<< "'\n" << std::flush;
}

void CourseRegistration::Clear(void)
{
	courseIdentifier = 0;
	studentIdentifier = 0;
	numCreditHours = 0;
	courseGrade = 0;
}

int CourseRegistration::InitBuffer(DelimFieldBuffer& Buffer)
{
	return TRUE;
}

int CourseRegistration::InitBuffer(LengthFieldBuffer& Buffer)
{
	return TRUE;
}

int CourseRegistration::InitBuffer(FixedFieldBuffer& Buffer)
{
	int result;
	result = Buffer.AddField(8);
	result = result && Buffer.AddField(8);
	result = result && Buffer.AddField(8);
	result = result && Buffer.AddField(8);
	return result;
}

int CourseRegistration::Pack(IOBuffer& Buffer) const
{
	int result;
	std::string temp = std::to_string(courseIdentifier);
	result = Buffer.Pack(temp.c_str());
	temp = std::to_string(studentIdentifier);
	result = result && Buffer.Pack(temp.c_str());
	temp = std::to_string(numCreditHours);
	result = result && Buffer.Pack(temp.c_str());
	temp = std::to_string(courseGrade);
	result = result && Buffer.Pack(temp.c_str());
	return result;
}

int CourseRegistration::Unpack(IOBuffer& Buffer)
{
	int result;
	char tempBuffer[32];
	result = Buffer.Unpack(tempBuffer);
	courseIdentifier = atoi(tempBuffer);
	result = result && Buffer.Unpack(tempBuffer);
	studentIdentifier = atoi(tempBuffer);
	result = result && Buffer.Unpack(tempBuffer);
	numCreditHours = atoi(tempBuffer);
	result = result && Buffer.Unpack(tempBuffer);
	courseGrade = atof(tempBuffer);
	return result;
}