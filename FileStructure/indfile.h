#include "recfile.h"
#include "tindbuff.h"
#include <strstream>
#include <string.h>
// template class to support direct read and write of records
// The template parameter RecType must support the following
//	int Pack (BufferType &); pack record into buffer
//	int Unpack (BufferType &); unpack record from buffer

template <class RecType>
class TextIndexedFile
{public:
	int Read (RecType & record); // read next record
	int Read (const char * key, RecType & record); // read by key
	int Append (const RecType & record);
	//int Delete(const RecType & record);//No found in the appendix
	int Update (const char * oldKey, const RecType & record);
	int Create (const char * name, int mode=ios::in|ios::out);
	int Open (const char * name, int mode=ios::in|ios::out);
	int Close ();
	TextIndexedFile (IOBuffer & buffer, 
		int keySize, int maxKeys = 100); 
	~TextIndexedFile (); // close and delete
protected:
	TextIndex Index;
	BufferFile IndexFile;
	TextIndexBuffer IndexBuffer;
	RecordFile<RecType> DataFile;
	char * FileName; // base file name for file
	int SetFileName(const char * fileName,
		char *& dataFileName, char *& indexFileName);
};

// template method bodies
template <class RecType>
int TextIndexedFile<RecType>::Read(RecType& record)
{
	int result = result = DataFile.Read(record, -1);
	return result;
}

template <class RecType>
int TextIndexedFile<RecType>::Read (const char * key, RecType & record)
{
	int ref = Index.Search(key);
	if (ref < 0) return -1;
	int result = DataFile . Read (record, ref);
	return result;
}

template <class RecType>
int TextIndexedFile<RecType>::Append (const RecType & record)
{
	const std::string key = record.Key();
	int ref = Index.Search(key.c_str());
	if (ref != -1) // key already in file
		return -1;
	ref = DataFile . Append(record);
	int result = Index . Insert (key.c_str(), ref);
	return ref;
}

template <class RecType>
int TextIndexedFile<RecType>::Update 
	(const char * oldKey, const RecType & record)
// Update is left as an exercise. 
//	It requires BufferFile::Update, and BufferFile::Delete
{	return -1;}


template <class RecType>
int TextIndexedFile<RecType>::SetFileName(const char * fileName,
	char *& dataFileName, char *& indexFileName)
// generate names for the data file and the index file
{
	if (FileName != 0) // object is already attached to a file
		return 0;
	// set FileName member
	FileName = _strdup(fileName);
	// generate real file names
	ostrstream dataName, indexName;
	dataName << FileName <<".dat"<<ends;
	indexName<< FileName <<".ind"<<ends;
	dataFileName = _strdup(dataName . str());
	indexFileName = _strdup(indexName . str());
	return 1;
}

template <class RecType>
int TextIndexedFile<RecType>::Create (const char * fileName, int mode)
// use fileName.dat and fileName.ind
{
	int result;
	char * dataFileName, * indexFileName;
	result = SetFileName (fileName, dataFileName, indexFileName);
	if (result == 0)
	{
		cout << "Use existing file : " << FileName << endl;;
		return 1;
	}
	else cout << "file names " << dataFileName << " " << indexFileName << endl;

	result = DataFile.Create (dataFileName, mode);
	if (!result)
	{
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Create (indexFileName, ios::out|ios::in);
	if (!result)
	{
		DataFile . Close(); // close the data file
		FileName = 0; // remove connection
		return 0;
	}
	return 1;
}
template <class RecType>
int TextIndexedFile<RecType>::Open (const char * fileName, int mode)
// open data and index file and read index file
{
	int result;
	char * dataFileName, * indexFileName;
	result = SetFileName (fileName, dataFileName, indexFileName);
	if (!result) return 0;
	// open files
	result = DataFile.Open (dataFileName, mode);
	if (!result)
	{
		FileName = 0; // remove connection
		return 0;
	}
	result = IndexFile.Open (indexFileName, ios::out);
	if (!result)
	{
		DataFile . Close(); // close the data file
		FileName = 0; // remove connection
		return 0;
	}
	// read index into memory
	result = IndexFile . Read ();
	if (result != -1)
	{
		result = IndexBuffer . Unpack (Index);
		if (result != -1) return 1;
	}
	// read or unpack failed!
	DataFile.Close();
	IndexFile.Close();
	FileName = 0;
	return 0;
}

template <class RecType>
int TextIndexedFile<RecType>::Close ()
{	int result;
	if (!FileName) return 0; // already closed!
	DataFile . Close();
	IndexFile . Rewind();
	IndexBuffer.Pack (Index);
	result = IndexFile . Write ();
	cout <<"result of index write: "<<result<<endl;
	IndexFile . Close ();
	FileName = 0;
	return 1;
}

template <class RecType>
TextIndexedFile<RecType>::TextIndexedFile (IOBuffer & buffer,
		int keySize, int maxKeys)
	:DataFile(buffer), Index (maxKeys), 
	IndexFile(IndexBuffer), IndexBuffer(keySize, maxKeys)
{
	FileName = 0;
}

template <class RecType>
TextIndexedFile<RecType>::~TextIndexedFile ()
{	Close(); }

