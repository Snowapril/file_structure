//testind.cc
//data file을 읽어서 index을 만들고 저장하는 코드
#include "textind.h"
#include "delim.h"
#include "buffile.h"
#include "recordng.h"
#include "tindbuff.h"
#include <iostream>
#include "recfile.h"
int RetrieveRecording (Recording & recording, char * key, 
			TextIndex & RecordingIndex, BufferFile & RecordingFile)
// read and unpack the recording, return TRUE if succeeds
{	int result;
	cout <<"Retrieve "<<key<<" at recaddr "<<RecordingIndex.Search(key)<<endl;
	result = RecordingFile . Read (RecordingIndex.Search(key));
	cout <<"read result: "<<result<<endl;
	if (result == -1) return FALSE;
	result = recording.Unpack (RecordingFile.GetBuffer());
	return result;
}

// make an index from a recording file
int IndexRecordingFile (char * myfile, TextIndex & RecordingIndex)
{
	Recording rec; 
	int recaddr, result;
	DelimFieldBuffer Buffer; // create a buffer
	BufferFile IndexingFile(Buffer); 
	result = IndexingFile . Create(myfile,ios::out);
	if (!result)
	{
		cout << "Unable to open file "<<myfile<<endl;
		return 0;
	}
	RecordFile<Recording> DataFile(Buffer);
	DataFile.Open("record2.dat", ios::in);//makerec-1.cpp 결과를 활용
	while (1) // loop until the read fails
	{
		recaddr = DataFile . Read (rec); // read data record into memory
		if (recaddr < 0) break;
		rec. Unpack (Buffer);
		RecordingIndex . Insert(rec.Key(), recaddr);
		cout << recaddr <<'\t'<<rec<<endl;
	}
	RecordingIndex . Print (cout);
	result = RetrieveRecording (rec, "LON2312", RecordingIndex, IndexingFile);
	cout <<"Found record: "<<rec;
}

int main (void)
{// first argument is the file name for the data file
	TextIndex RecordingIndex (10);
	char* filename = "record2.dat";
	IndexRecordingFile (filename, RecordingIndex);


	// store the index in a file
	TextIndexBuffer IndexBuffer (12, 10);// 12 byte key, 10 keys
	BufferFile IndexFile (IndexBuffer);
	IndexBuffer.Pack(RecordingIndex);
	IndexFile . Create ("recindex.dat", ios::out);
	IndexFile . Write ();
	IndexFile.Close();

	system("pause");
	return 1;
}

