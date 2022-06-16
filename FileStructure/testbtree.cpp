//#pragma warning(disable:4996)
//template은  **.h와 **.cpp를 분리하지 않고 **.h로 합쳐야 link error를 해결
//#include "btnode.h"

#include <iostream>
#include <string>
#include "btree.h"

using namespace std;

const char * keys = "CSDTAMPIBWNGURKEHOLJYQZFXV";

const int BTreeSize = 4;
int main(int argc, char * argv) {
	int result, i;
	BTree <char> bt(BTreeSize);
	cout << "input data = " << keys << endl;
	result = bt.Create("btree.dat", ios::out);
	if (!result) { 
		cout << "Please delete testbt.dat" << endl; 
		system("pause");
		return 0; 

	}
	for (i = 0; i<26; i++)
	{
		cout << "Inserting " << keys[i] << endl;
		result = bt.Insert(keys[i], i);
		bt.Print(cout);
	}
	bt.Search(1, 1);
	bt.Remove('L');//구현 - redistribute와 merge를 구현한다.
	//bt.InorderTraversal();
	system("pause");
	return 1;
}