#include "stdafx.h"
#include "CppUnitTest.h"
#include "Btree.h"
#include <time.h>
using namespace tree;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	int calcuateDepth(int size) {
		int t = 0;
		if (size > 0) {
			t = 1;
			while ((size = (size / 2)) >= 1) {
				t++;
			}
		}
		return t;
	}

	Btree<int, int> insertInts(std::vector<int> v) {
		Btree<int, int> btree = Btree<int, int>();
		for (auto &i : v) {
			btree.insert(i, i);
		}
		return btree;
	}

	Btree<int, int> insertRandom(int size, int max) {
		std::vector<int> v;
		for (int i = 0; i < size; i++) {
			int r = rand() % max;
			v.push_back(r);
		}
		return insertInts(v);
	}

	Btree<int, int> insertRandom() {
		return insertRandom(rand() % 1000, 100000);
	}

	Btree<int, int> btree;

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_CLASS_INITIALIZE(init)
		{
			srand(time(NULL));
		}
		TEST_METHOD(TestTreeSimple)
		{
			btree = insertInts({ 10, 4, 7 });
			Assert::AreEqual(3, btree.size());
			Assert::IsNotNull(btree.get(4));
			Assert::IsNull(btree.get(44));
			Assert::IsTrue(btree.contains(7));
			Assert::IsFalse(btree.contains(77));
			Assert::IsTrue(btree.containsAll({ 10, 4, 7 }));
			Assert::AreEqual(10, btree.getVal(10));
			Assert::ExpectException<exception>([&]
			{
				btree.getVal(1);
			});
		}
		TEST_METHOD(TestTree)
		{
			btree = insertInts({ 0, 2, 5, 10, 15, 20, 12, 14, 13, 25, 0, 2, 5, 10, 15, 20, 12, 14, 13, 25 });
			Assert::AreEqual(10, btree.size());
		}
		TEST_METHOD(TestTreeDepth)
		{
			btree = insertRandom();
			Assert::IsTrue(btree.depth() >= calcuateDepth(btree.size()));
		}
		TEST_METHOD(TestTreeIterate)
		{
			btree = insertInts({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
			Btree<int, int>::Node *beginning = btree.beginning();
			Btree<int, int>::Node *end = btree.end();
			Assert::AreEqual(0, beginning->getVal());
			Assert::AreEqual(10, end->getVal());
			Btree<int, int>::Node *ptr = beginning;
			int i = 0;
			while (i <= 10) {
				Assert::AreEqual(i, ptr->getVal());
				ptr = ptr->next();
				i++;
			}
			i = 10;
			ptr = end;
			while (i >= 0) {
				Assert::AreEqual(i, ptr->getVal());
				ptr = ptr->previous();
				i--;
			}
		}
		TEST_METHOD(TestTreeClear)
		{
			btree = insertRandom();
			btree.clear();
			Assert::AreEqual(0, btree.size());
			Assert::IsTrue(btree.isEmpty());
			Assert::IsNull(btree.beginning());
			Assert::IsNull(btree.end());
		}
		TEST_METHOD(TestTreeDelete)
		{
			//std::vector<int> v = { 0, 1, 2, 3 };
			std::vector<int> v = { 0, 1 };
			btree = insertInts(v);
			for (auto &i : v) {
				Btree<int, int>::Node *node = btree.remove(i);
				Assert::IsNotNull(node);
				Assert::AreEqual(i, node->getVal());
			}
			Assert::AreEqual(0, btree.size());
			//while (!btree.isEmpty()) {
			//	btree.remove(10);
			//}
			//Assert::AreEqual(0, btree.size());
			//Assert::IsTrue(btree.isEmpty());
			//Assert::IsNull(btree.beginning());
			//Assert::IsNull(btree.end());
		}
	};
}