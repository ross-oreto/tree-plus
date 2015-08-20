#pragma once

#include <iostream>
#include <sstream>
using namespace std;

namespace tree {
	template <typename X> int cmp(X s1, X s2)
	{
		if (s1 < s2) return -1;
		else if (s1 > s2) return 1;
		else return 0;
	}

	template <typename K, typename V>
	class Btree {
	public:
		friend class Node {
		public:
			enum Type
			{
				ROOT, LEFT, RIGHT
			};

			Node(K key, V val);
			K getKey();
			V getVal();
			Node* getLeft();
			Node* getRight();
			Node* getParent();
			Type getType();
			string toString();

		protected:
			K key;
			V val;
			Node *left;
			Node *right;
			Node *parent;
			int balance;
			Type type;

			int insert(Node*);
			Node* get(K);

			bool hasNoChildren();
			bool hasTwoChildren();
			bool hasChild();
			bool hasRightChild();
			bool hasLeftChild();
			bool isRightChild();
			bool isLeftChild();
			bool isRoot();
			bool hasParent();
			void replace(Node*);
			bool leansRight();
			bool leansLeft();
		private:
			Node* attachLeftNode(Node*, bool addWeight = true);
			Node* attachRightNode(Node*, bool addWeight = true);
			Node* attachNode(Node*, Type);
			Node* detachLeftNode();
			Node* detachRightNode();
			Node* rotateLeft();
			Node* rotateRight();
			void setBalance();
		};

		Btree();
		bool isEmpty();
		int size();
		Btree insert(K, V);
		Node* getRoot();
		Node* get(K);
		V getVal(K);
		bool contains(K);
		void print();

	protected:
		bool DEL = false;
		Node *root;
		void count(Node* node, int *count);
		void print(Node* node);
	};

	template <typename K, typename V>
	Btree<K, V>::Btree() {
		root = NULL;
	}

	template <typename K, typename V>
	int Btree<K, V>::size() {
		int nodes = 0;
		if (!isEmpty()) {
			count(root, &nodes);
		}
		return nodes;
	}

	template <typename K, typename V>
	bool Btree<K, V>::isEmpty() {
		return root == NULL;
	}

	template <typename K, typename V>
	void Btree<K, V>::count(Node* node, int *nodes) {
		if (node != NULL) {
			count(node->getLeft(), nodes);
			*nodes += 1;
			count(node->getRight(), nodes);
		}
	}

	template <typename K, typename V>
	void Btree<K, V>::print() {
		print(root);
	}

	template <typename K, typename V>
	void Btree<K, V>::print(Node* node) {
		if (node != NULL) {
			print(node->getLeft());
			cout << node->toString().append("\n");
			print(node->getRight());
		}
	}

	/************************************************
	Description: inserts key and val into the tree
	key: The value to key the node on.
	val: Value associated with key.
	************************************************/
	template <typename K, typename V>
	Btree<K, V> Btree<K, V>::insert(K key, V value) {
		this->DEL = false;
		Btree<K, V>::Node *newNode = new Node(key, value);
		if (isEmpty()) {
			this->root = newNode;
		} 
		else {
			root->insert(newNode);
			while (root->getType() != Btree<K, V>::Node::Type::ROOT) {
				root = root->getParent();
			}
		}
		return *this;
	}

	template <typename K, typename V>
	int Btree<K, V>::Node::insert(Node *newNode) {
		if (cmp<K>(newNode->getKey(), getKey()) < 0) {
			if (getLeft() == NULL) {
				attachLeftNode(newNode);
			}
			else {
				int weight = abs(getLeft()->insert(newNode));
				if (weight == 0) return weight;
				balance = balance - weight;
			}
		}
		else if (cmp<K>(newNode->getKey(), getKey()) > 0) {
			if (getRight() == NULL) {
				attachRightNode(newNode);
			}
			else {
				int weight = abs(getRight()->insert(newNode));
				if (weight == 0) return weight;
				balance = balance + weight;
			}
		}
		else {
			replace(newNode);
			return 0;
		}
		if (balance < -1) rotateRight()->balance; 
		if (balance > 1) rotateLeft()->balance;
		return balance;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::rotateLeft() {
		Node *node = detachRightNode();
		if (node->leansLeft()) {
			node = node->rotateRight();
		}
		if (isRoot()) node->type = Type::ROOT;
		else if(hasParent()){
			parent->attachNode(node, type);
		}
		if (node->hasLeftChild()) {
			attachRightNode(node->detachLeftNode());
		}
		node->attachLeftNode(this);
		if (abs(balance) < 2) node->balance = balance * -1;
		setBalance();
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::rotateRight() {
		Node *node = detachLeftNode();
		if (node->leansRight()) {
			node = node->rotateLeft();
		}
		if (isRoot()) node->type = Type::ROOT;
		else if (hasParent()) {
			parent->attachNode(node, type);
		}
		if (node->hasRightChild()) {
			attachLeftNode(node->detachRightNode());
		} 
		node->attachRightNode(this);
		if (abs(balance) < 2) node->balance = balance * -1;
		setBalance();
		return node;
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::setBalance() {
		if (hasNoChildren() || hasTwoChildren()) balance = 0;
		else if (hasLeftChild()) balance = -1;
		else balance = 1;
	}

	template <typename K, typename V>
	Btree<K, V>::Node::Node(K key, V val) {
		this->key = key;
		this->val = val;
		left = right = parent = NULL;
		type = Type::ROOT;
		balance = 0;
	}

	template <typename K, typename V>
	string Btree<K, V>::Node::toString() {
		string sep = ", ";
		string s = "[ "+std::to_string(key).append(sep).append("type: ");

		switch (type)
		{
		case Type::LEFT: s.append("left child"); break;
		case Type::RIGHT: s.append("right child"); break;
		case Type::ROOT: s.append("root"); break;
		default:
			break;
		}
		if (!isRoot()) {
			s.append(sep).append("parent: ").append(std::to_string(getParent()->getKey()));
		}
		s.append(sep).append("balance: ").append(std::to_string(balance)).append(" ]");
		return s;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::get(K key) {
		Node *node = NULL;
		if (!isEmpty()) node = root->get(key);
		return node;
	}

	template <typename K, typename V>
	V Btree<K, V>::getVal(K key) {
		Node *node = get(key);
		if (node == NULL) {
			std::stringstream ostr;
			ostr << "key not found exception: " << std::to_string(key);
			throw(exception(ostr.str().c_str()));
		}
		return node->getVal();
	}

	template <typename K, typename V>
	bool Btree<K, V>::contains(K key) {
		Node *node = NULL;
		bool contains = false;
		if (!isEmpty()) node = root->get(key);
		if (node != NULL) contains = true;
		return contains;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::get(K key) {
		Node *node = NULL;
		if (cmp<K>(key, getKey()) < 0) {
			if(hasLeftChild()) node = left->get(key);
		}
		else if (cmp<K>(key, getKey()) > 0) {
			if (hasRightChild()) node = right->get(key);
		}
		else {
			node = this;
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::getRoot() {
		return this->root;
	}

	template <typename K, typename V>
	typename V Btree<K, V>::Node::getVal() {
		return val;
	}

	template <typename K, typename V>
	typename K Btree<K, V>::Node::getKey() {
		return key;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node::Type Btree<K, V>::Node::getType() {
		return type;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachLeftNode(Node *node, bool addWeight) {
		left = node;
		node->type = Type::LEFT;
		node->parent = this;
		if (addWeight) {
			balance -= 1;
		}
		return this;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachRightNode(Node *node, bool addWeight) {
		right = node;
		node->type = Type::RIGHT;
		node->parent = this;
		if (addWeight) {
			balance += 1;
		}
		return this;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachNode(Node *node, Type type) {
		switch (type)
		{
		case Type::LEFT:
			attachLeftNode(node, false);
			break;
		case Type::RIGHT:
			attachRightNode(node, false);
			break;
		default:
			break;
		}
		return this;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::detachLeftNode() {
		Node *node = NULL;
		if (hasLeftChild()) {
			node = left;
			left = NULL;
			node->parent = NULL;
			return node;
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::detachRightNode() {
		Node *node = NULL;
		if (hasRightChild()) {
			node = right;
			right = NULL;
			node->parent = NULL;
		}
		return node;
	}

	template <typename K, typename V>
	typename void Btree<K, V>::Node::replace(Node *node) {
		val = node->val;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::getLeft() {
		return left;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::getRight() {
		return right;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::getParent() {
		return parent;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasParent() {
		return parent != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasTwoChildren() {
		return right != NULL && left != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasChild() {
		return right != NULL || left != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasNoChildren() {
		return right == NULL && left == NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasRightChild() {
		return right != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasLeftChild() {
		return left != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isRightChild() {
		return type == Type::RIGHT;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isLeftChild() {
		return type == Type::LEFT;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isRoot() {
		return type == Type::ROOT;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::leansLeft() {
		return balance < 0;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::leansRight() {
		return balance > 0;
	}
}
