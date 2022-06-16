#include "strlist.h"
//StringListProcess
//allow any number of the input list(number of items)
int main ()
{
	StringListProcess List(2);// declare process with 2 lists
	cout << "List.Match2Lists (list1.txt,list2.txt,match.txt)" << endl;
	List.Match2Lists ("list1.txt","list2.txt","match.txt");
	cout << endl<<"List.Merge2Lists (list1.txt,list2.txt,match.txt)" << endl;
	List.Merge2Lists ("list1.txt","list2.txt","merge.txt");
	system("pause");
	return 1;
}

