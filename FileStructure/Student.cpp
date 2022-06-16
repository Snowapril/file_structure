#include "Student.h"

std::ostream& operator<<(std::ostream& stream, const Student& student)
{
	student.Print(stream);
	return stream;
}

std::istream& operator>>(std::istream& stream, Student& student)
{
	std::cout << "Enter the identifier: "; stream >> student.identifier;
	std::cout << "Enter the name: "; stream >> student.name;
	std::cout << "Enter the address: "; stream >> student.address;
	std::cout << "Enter the first enroll date: "; stream >> student.firstEnrollDate;
	std::cout << "Enter the num credit hours: "; stream >> student.numCreditHours;
	return stream;
}

Student::Student(int identifier, std::string name, std::string address,
				 std::string firstEnrollDate, int numCreditHours)
	: identifier(identifier), name(std::move(name)), address(std::move(address)),
	  firstEnrollDate(std::move(firstEnrollDate)), numCreditHours(numCreditHours)
{
	// Do nothing
}

Student& Student::operator=(const Student& other)
{
	if (this == &other)
	{
		identifier = other.identifier;
		name = other.name;
		address = other.address;
		firstEnrollDate = other.firstEnrollDate;
		numCreditHours = other.numCreditHours;
	}
	return *this;
}

void Student::incrNumCreditHours(void)
{
	++numCreditHours;
}

void Student::Print(std::ostream& stream, const char* label) const
{
	if (label == 0) stream << "Student:";
	else stream << label;
	stream << "	Identifier	'"			<< identifier		<< "'\n"
		<< "		Name		'"		<< name				<< "'\n"
		<< "		Address		'"		<< address			<< "'\n"
		<< "\t   First Enroll Date	'"	<< firstEnrollDate	<< "'\n"
		<< "\t    Num Credit Hours	'"	<< numCreditHours	<< "'\n" << std::flush;
}

ostream& Student::PrintHeader(ostream& stream)
{// print the report header for this account
	stream << identifier << '\t' << name << endl;
	return stream;
}

void Student::Clear(void)
{
	identifier = 0;
	name.clear();
	address.clear();
	firstEnrollDate.clear();
	numCreditHours = 0;
}

int Student::InitBuffer(DelimFieldBuffer& Buffer)
{
	return TRUE;
}

int Student::InitBuffer(LengthFieldBuffer& Buffer)
{
	return TRUE;
}

int Student::InitBuffer(FixedFieldBuffer& Buffer)
{
	int result;
	result = Buffer.AddField(16);
	result = result && Buffer.AddField(16);
	result = result && Buffer.AddField(16);
	result = result && Buffer.AddField(16);
	result = result && Buffer.AddField(16);
	return result;
}

int Student::Pack(IOBuffer& Buffer) const
{
	int result;
	std::string temp = std::to_string(identifier);
	result = Buffer.Pack(temp.c_str());
	result = result && Buffer.Pack(name.c_str());
	result = result && Buffer.Pack(address.c_str());
	result = result && Buffer.Pack(firstEnrollDate.c_str());
	temp = std::to_string(numCreditHours);
	result = result && Buffer.Pack(temp.c_str());
	return result;
}

int Student::Unpack(IOBuffer& Buffer)
{
	int result;
	char tempBuffer[64];
	result = Buffer.Unpack(tempBuffer);
	identifier = atoi(tempBuffer);
	result = result && Buffer.Unpack(tempBuffer);
	name = std::string(tempBuffer);
	result = result && Buffer.Unpack(tempBuffer);
	address = std::string(tempBuffer);
	result = result && Buffer.Unpack(tempBuffer);
	firstEnrollDate = std::string(tempBuffer);
	result = result && Buffer.Unpack(tempBuffer);
	numCreditHours = atoi(tempBuffer);
	return result;
}

std::string Student::Key() const
{
	return std::to_string(identifier);
}