#include <algorithm>
#include <iostream>
using namespace std;
#define COUNT 10

#ifndef AVL_TREE
#define AVL_TREE

class Exist {};
class EmptyTree {};
class NotFound {};

template <class T, class ID>
class AVLTree
{
private:
	AVLTree<T, ID>* right;
	AVLTree<T, ID>* left;
	AVLTree<T, ID>* parent;
	int height;
	int balance;
	T m_data;
	ID m_id;

	void swap(AVLTree<T, ID>* first, AVLTree<T, ID>* second);
	AVLTree<T, ID>* find(ID id);
	AVLTree<T, ID>* getMostLeft();
	void updateHeightBalance();
	void removeRoll();
	void insertRoll();
	void rollLL();
	void rollLR();
	void rollRR();
	void rollRL();
	bool isLeaf() {
		return (!right && !left);
	}

public:
	AVLTree() : right(nullptr), left(nullptr), parent(nullptr), height(-1), balance(0), m_data(), m_id() {}
	AVLTree(AVLTree<T, ID>* parent, T data, ID id) : right(nullptr), left(nullptr), parent(parent), height(0), balance(0), m_data(data), m_id(id) {}
	~AVLTree() = default;
	void deleteTree(AVLTree<T, ID>* root);
	AVLTree<T, ID>* getRoot() {
		if (height == -1) {
			throw EmptyTree();
		}
		if (!parent) {
			return this;
		}
		return parent->getRoot();
	}

	// Function to print binary tree in 2D
// It does reverse inorder traversal
	void print2DUtil(AVLTree<int, int>* root, int space)
	{
		// Base case
		if (root == NULL)
			return;

		// Increase distance between levels
		space += COUNT;

		// Process right child first
		print2DUtil(root->right, space);

		// Print current node after space
		// count
		cout << endl;
		for (int i = COUNT; i < space; i++)
			cout << " ";
		cout << root->m_id << "\n";

		// Process left child
		print2DUtil(root->left, space);
	}

	// Wrapper over print2DUtil()
	void print2D(AVLTree<int, int>* root)
	{
		// Pass initial space count as 0
		print2DUtil(root, 0);
	}

	void printTreeInOrder() {
		if (left) {
			left->printTreeInOrder();
		}
		std::cout << "id:" << m_id << "balance: " << balance << "\n";
		if (right)
		{
			right->printTreeInOrder();
		}
	}
	void printTreePreOrder() {
		std::cout << "id:" << m_id << "balance: " << balance << "\n";

		if (left) {
			left->printTreePreOrder();
		}
		if (right)
		{
			right->printTreePreOrder();
		}
	}

	void insert(T data, ID id) {
		if (height == -1) {
			m_data = data;
			m_id = id;
			height++;
			return;
		}
		if (m_id < id) {
			if (!right) {
				right = new AVLTree<T, ID>(this, data, id);
				right->insertRoll();
			}
			else {
				right->insert(data, id);
			}
		}
		else if (m_id == id) {
			throw Exist();
		}
		else {
			if (!left) {
				left = new AVLTree<T, ID>(this, data, id);
				left->insertRoll();
			}
			else {
				left->insert(data, id);
			}
		}
	}



	void remove(ID id) {
		AVLTree<T, ID>* toDelete = find(id);
		if (toDelete->right && toDelete->left) {
			AVLTree<T, ID>* toSwich = toDelete->right->getMostLeft();
			swap(toDelete, toSwich);
		}
		else if (toDelete->left) {
			swap(toDelete, toDelete->left);
		}
		else if (toDelete->right) {
			swap(toDelete, toDelete->right);
		}
		else if (!toDelete->parent) {
			deleteTree(toDelete);
			return;
		}
		else {
			if (toDelete->m_id < toDelete->parent->m_id) {
				toDelete->parent->left = nullptr;
			}
			else {
				toDelete->parent->right = nullptr;
			}
		}
		AVLTree<T, ID>* parent = toDelete->parent;
		delete toDelete;
		parent->removeRoll();

	}

};

template <class T, class ID>
AVLTree<T, ID>* AVLTree<T, ID>::find(ID id) {
	if (height == -1) {
		throw EmptyTree();
	}
	if (m_id == id) {
		return this;
	}
	if (m_id < id) {
		if (!right) {
			throw NotFound();
		}
		return right->find(id);
	}
	if (!left) {
		throw NotFound();
	}
	return left->find(id);
}

template <class T, class ID>
void AVLTree<T, ID>::swap(AVLTree<T, ID>* first, AVLTree<T, ID>* second) {
	AVLTree<T, ID>* tempRight = first->right;
	AVLTree<T, ID>* tempLeft = first->left;
	AVLTree<T, ID>* tempParent = first->parent;
	first->right = second->right;
	first->left = second->left;
	first->parent = second->parent;
	second->right = tempRight;
	second->left = tempLeft;
	second->parent = tempParent;
	if (first->m_id < tempParent->m_id) {
		tempParent->left = second;
	}
	else {
		tempParent->right = second;
	}
	if (second->m_id < second->parent->m_id) {
		second->parent->left = first;
	}
	else {
		second->parent->right = first;
	}
	if (tempRight) {
		tempRight->parent = second;
	}
	if (tempLeft) {
		tempLeft->parent = second;
	}
	if (first->right) {
		first->right->parent = first;
	}
	if (first->left) {
		first->left->parent = first;
	}
}


template <class T, class ID>
void AVLTree<T, ID>::updateHeightBalance() {
	if (left && right) {
		height = std::max(left->height, right->height) + 1;
		balance = left->height - right->height;
	}
	else if (left) {
		height = left->height + 1;
		balance = height;
	}
	else if (right) {
		height = right->height + 1;
		balance = height * (-1);
	}
	else {
		height = 0;
		balance = 0;
	}
}


template <class T, class ID>
void AVLTree<T, ID>::insertRoll() {
	updateHeightBalance();
	if (balance > -2 && balance < 2) {
		if (parent) {
			parent->insertRoll();
		}
	}
	else {
		if (balance == 2) {
			if (left->balance == 1) {
				rollLL();
			}
			else
			{
				rollLR();
			}
		}
		else if (balance == -2) {
			if (right->balance == 1) {
				rollRL();
			}
			else
			{
				rollRR();
			}

		}
	}
}


template <class T, class ID>
void  AVLTree<T, ID> ::rollLL() {
	left->parent = parent;
	parent = left;
	if (left->parent) {
		if (left->parent->m_id < left->m_id) {
			left->parent->right = left;
		}
		else {
			left->parent->left = left;
		}
	}
	left = left->right; //now B_R points at A_R and A is B parent
	if (left) {
		left->parent = this;
	}
	parent->right = this;
	updateHeightBalance();
	parent->updateHeightBalance();
}


template <class T, class ID>
void  AVLTree<T, ID> ::rollRR() {
	right->parent = parent;
	parent = right;
	if (right->parent) {
		if (right->parent->m_id < right->m_id) {
			right->parent->right = right;
		}
		else {
			right->parent->left = right;
		}
	}
	right = right->left;
	if (right) {
		right->parent = this;
	}
	parent->left = this;
	updateHeightBalance();
	parent->updateHeightBalance();

}


template <class T, class ID>
void  AVLTree<T, ID> ::rollLR() {
	AVLTree<T, ID>* A = left;
	AVLTree<T, ID>* B = A->right;
	AVLTree<T, ID>* BL = B->left;
	AVLTree<T, ID>* BR = B->right;
	B->parent = parent;
	parent = B;
	A->parent = B;
	if (BL) {
		BL->parent = A;
	}
	if (BR) {
		BR->parent = this;
	}
	if (B->parent) {
		if (B->parent->m_id < B->m_id) {
			B->parent->right = B;
		}
		else {
			B->parent->left = B;
		}
	}
	B->left = A;
	B->right = this;
	this->left = BR;
	A->right = BL;
	A->updateHeightBalance();
	updateHeightBalance();
	B->updateHeightBalance();
}


template <class T, class ID>
void  AVLTree<T, ID> ::rollRL() {
	AVLTree<T, ID>* A = right;
	AVLTree<T, ID>* B = A->left;
	AVLTree<T, ID>* BL = B->left;
	AVLTree<T, ID>* BR = B->right;
	B->parent = parent;
	parent = B;
	A->parent = B;
	if (BL) {
		BL->parent = A;
	}
	if (BR) {
		BR->parent = this;
	}
	if (B->parent) {
		if (B->parent->m_id < B->m_id) {
			B->parent->right = B;
		}
		else {
			B->parent->left = B;
		}
	}
	B->left = this;
	B->right = A;
	right = BL;
	A->left = BR;
	A->updateHeightBalance();
	updateHeightBalance();
	B->updateHeightBalance();
}


template <class T, class ID>
void  AVLTree<T, ID> ::removeRoll() {
	updateHeightBalance();
	if (balance == 2) {
		if (left->balance == 1) {
			rollLL();
		}
		else {
			rollLR();
		}
	}
	else if (balance == -2) {
		if (right->balance == 1) {
			rollRL();
		}
		else {
			rollRR();
		}
	}
	if (parent) {
		parent->removeRoll();
	}
}

template <class T, class ID>
void AVLTree<T, ID>::deleteTree(AVLTree<T, ID>* root) {
	if (!root) {
		throw EmptyTree();
	}
	if (root->left) {
		if (!root->left->isLeaf()) {
			deleteTree(root->left);
		}
		delete root->left;
	}
	if (root->right) {
		if (!root->right->isLeaf()) {
			deleteTree(root->right);
		}
		delete root->right;
	}
	delete root;
}


template <class T, class ID>
AVLTree<T, ID>* AVLTree<T, ID> ::getMostLeft() {
	if (!left) {
		return this;
	}
	return left->getMostLeft();
}

#endif // AVL_TREE