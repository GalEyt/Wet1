#include <algorithm>
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
	AVLTree<T, ID>* getMostLeft(AVLTree<T, ID>* node);
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
	AVLTree() : right(nullptr), left(nullptr), parent(nullptr), height(-1), balance(0), m_data(nullptr) {}
	AVLTree(AVLTree<T, ID>* parent, T data, ID id) : right(nullptr), left(nullptr), parent(parent), height(0), balance(0), m_data(data), m_id(id) {}
	~AVLTree() = default;

	void insert(T data, ID id) {
		if (!m_data) {
			m_data = data;
			height++;
		}
		if (m_data < data) {
			if (!right) {
				right = new AVLTree<T, ID>(this, data, id);
				right->insertRoll();
			}
			else {
				right->insert(data);
			}
		}
		else if (m_data == data) {
			throw Exist();
		}
		else {
			if (!left) {
				left = new AVLTree<T, ID>(this, data, id);
				left->insertRoll();
			}
			else {
				left->insert(data);
			}
		}
	}



	void remove(ID id) {
		AVLTree<T, ID>* toDelete = find(id);
		if (toDelte->right && toDelte->left) {
			AVLTree<T, ID>* toSwich = getMostLeft(toDelete->getMostLeft());
			swap(toDelete, toSwich);
		}
		else if (toDelete->left) {
			swap(toDelete, toDelete->left);
		}
		else if (toDelete->right) {
			swap(toDelete, toDelete->right);
		}
		else if (!toDelete->parent) {
			~AVLTree();
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
	if (!m_data) {
		throw EmptyTree();
	}
	if (m_id == id) {
		return this
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
AVLTree<T, ID>* AVLTree<T, ID>::getMostLeft(AVLTree<T, ID>* node) {
	if (!node->left) {
		return node;
	}
	return getMostLeft(node->left);
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
		balance = height;
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
		else {
			if (left->balance == 1) {
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
	left->parent = this;
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
	right->parent = this;
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
	C->parent = B;
	A->parent = B;
	BL->parent = A;
	BR->parent = C;
	if (B->parent->m_id < B->m_id) {
		B->parent->right = B;
	}
	else {
		B->parent->left = B;
	}
	B->left = A;
	B->right = C;
	C->left = BR;
	A->right = BL;
	A->updateHeightBalance();
	C->updateHeightBalance();
	B->updateHeightBalance();
}


template <class T, class ID>
void  AVLTree<T, ID> ::rollRL() {
	AVLTree<T, ID>* A = right;
	AVLTree<T, ID>* B = A->left;
	AVLTree<T, ID>* BL = B->left;
	AVLTree<T, ID>* BR = B->right;
	B->parent = parent;
	C->parent = B;
	A->parent = B;
	BL->parent = A;
	BR->parent = C;
	if (B->parent->m_id < B->m_id) {
		B->parent->right = B;
	}
	else {
		B->parent->left = B;
	}
	B->left = C;
	B->right = A;
	C->right = BL;
	A->left = BR;
	A->updateHeightBalance();
	C->updateHeightBalance();
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
void deleteTree(AVLTree<T, ID>* root) {
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

#endif // AVL_TREE