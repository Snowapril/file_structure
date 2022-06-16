// mastrans.h
#ifndef MASTRANS_H
#define MASTRANS_H

#include "coseq.h"
#include <iostream>

template <class ItemType>
class MasterTransactionProcess: public CosequentialProcess<ItemType>
// a cosequential process that supports master/transaction processing
{public:
	MasterTransactionProcess (); //constructor
	virtual int ProcessNewMaster ()=0;//  when new master read
	virtual int ProcessCurrentMaster ()=0;// each transaction for a master
	virtual int ProcessEndMaster ()=0;// after all transactions for a master
	virtual int ProcessTransactionError ()=0;// no master for transaction

	int InitializeList(int ListNumber, const char* ListName) override = 0;
	int InitializeOutput(const char* OutputListName) override = 0;
	int NextItemInList(int ListNumber) override = 0;
	//advance to next item in this list
	ItemType Item(int ListNumber) override = 0;
	// return current item from this list
	int ProcessItem(int ListNumber) override = 0; // process the item in this list
	int FinishUp() override = 0; // complete the processing

	// cosequential processing of master and transaction records
	int PostTransactions (const char * MasterFileName, const char * TransactionFileName, const char * OutputListName);
};

template <class ItemType>
MasterTransactionProcess<ItemType>::MasterTransactionProcess ()
: CosequentialProcess<int>(2)
{}

template <class ItemType>
int MasterTransactionProcess<ItemType>::PostTransactions (const char * MasterFileName, const char * TransactionFileName, const char * OutputListName)
{
	int MoreMasters, MoreTransactions; // true if more items in particular list
	InitializeList (1, MasterFileName);
	InitializeList (2, TransactionFileName);
	InitializeOutput (OutputListName);
	MoreMasters = NextItemInList(1);
	MoreTransactions = NextItemInList(2);
	if (MoreMasters) ProcessNewMaster(); // process first master
	std::cout << "Master::" << Item(1) << "  " << Item(2) << std::endl;
	while (MoreMasters || MoreTransactions) {// if either file has more
		if (Item(1) < Item(2))
		{// finish this master record
			ProcessEndMaster();
			MoreMasters = NextItemInList(1);
			if (MoreMasters) ProcessNewMaster(); 
		}
		else if (Item(1) == Item(2)) // Transaction matches Master
		{
			ProcessCurrentMaster(); // another transaction for the master
			ProcessItem(2);// output transaction record
			MoreTransactions = NextItemInList(2);
		}
		else // Item(1) > Item(2)
		{// transaction with no master
			ProcessTransactionError();
			MoreTransactions = NextItemInList(2);
		}
	}
	FinishUp();
	return 1;

}

#endif
