// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Btree.h"

using namespace tree;

int main()
{
	Btree<int, string> btree = Btree<int, string>();
	btree.insert(1, "test");
    return 0;
}


