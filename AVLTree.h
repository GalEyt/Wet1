#include <algorithm>
#include <iostream>
using namespace std;
#define COUNT 10

#ifndef AVL_TREE
#define AVL_TREE

class Exist
{
};
class EmptyTree
{
};
class NotFound
{
};

template <class T, class ID>
class AVLTree
{
private:
	AVLTree<T, ID> *right;
	AVLTree<T, ID> *left;
	AVLTree<T, ID> *parent;
	int height;
	int balance;
	T m_data;
	ID m_id;

	void swap(AVLTree<T, ID> *other);
	AVLTree<T, ID> *find(ID id);
	AVLTree<T, ID> *getMostLeft();
	void updateHeightBalance();
	void removeRoll();
	void insertRoll();
	void rollLL();
	void rollLR();
	void rollRR();
	void rollRL();
	bool isLeaf()
	{
		return (!right && !left);
	}
	

	bool isLeftSon()
	{
		// assert parent
		if (parent->left == this)
		{
			return true;
		}
		return false;
	}

	AVLTree<T, ID> *removeLeaf()
	{
		if (!parent)
		{
			deleteTree(this);
			return nullptr;
		}
		else
		{
			if (isLeftSon())
			{
				parent->left = nullptr;
			}
			else
			{
				parent->right = nullptr;
			}
		}
		AVLTree<T, ID> *tmpParent = parent;
		delete this;
		return tmpParent;
	}

	AVLTree<T, ID> *removeOnlySon()
	{
		AVLTree<T, ID> *tmp;
		if (!parent)
		{
			if (right)
			{
				right->parent = nullptr;
				tmp = right;
			}
			else
			{
				left->parent = nullptr;
				tmp = left;
			}
		}
		else
		{
			if (isLeftSon())
			{
				if (right)
				{
					parent->left = right;
					right->parent = parent;
				}
				else
				{
					parent->left = left;
					left->parent = parent;
				}
				tmp = parent;
			}
			else
			{
				if (right)
				{
					parent->right = right;
					right->parent = parent;
				}
				else
				{
					parent->right = left;
					left->parent = parent;
				}
				tmp = parent;
			}
		}
		delete this;
		return tmp;
	}

	AVLTree<T, ID> *removeTwoSons()
	{
		AVLTree<T, ID> *toSwitch = right->getMostLeft();
		swap(toSwitch);
		if (toSwitch->isLeaf())
		{
			return toSwitch->removeLeaf();
		}
		return toSwitch->removeOnlySon();
	}

	AVLTree<T, ID> *removeHelper(ID id)
	{
		AVLTree<T, ID> *toRoll;
		AVLTree<T, ID> *toDelete = find(id);
		if (toDelete->isLeaf())
		{
			toRoll = toDelete->removeLeaf();
			if (!toRoll)
			{
				return new AVLTree<T, ID>;
			}
		}
		else if (toDelete->right && toDelete->left)
		{
			toRoll = toDelete->removeTwoSons();
		}
		else
		{
			toRoll = toDelete->removeOnlySon();
		}
		toRoll->removeRoll();
		return toRoll;
	}

public:
	AVLTree() : right(nullptr), left(nullptr), parent(nullptr), height(-1), balance(0), m_data(), m_id() {}
	AVLTree(AVLTree<T, ID> *parent, T data, ID id) : right(nullptr), left(nullptr), parent(parent), height(0), balance(0), m_data(data), m_id(id) {}
	~AVLTree() = default;
	void deleteTree(AVLTree<T, ID> *root);
	AVLTree<T, ID> *getRoot()
	{

		if (!parent || height == -1)
		{
			return this;
		}
		return parent->getRoot();
	}

	// Function to print binary tree in 2D
	// It does reverse inorder traversal
	void print2DUtil(AVLTree<int, int> *root, int space)
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
	void print2D(AVLTree<int, int> *root)
	{
		// Pass initial space count as 0
		print2DUtil(root, 0);
	}

	void printTreeInOrder()
	{
		if (left)
		{
			left->printTreeInOrder();
		}
		std::cout << m_id << " "; // << "balance: " << balance << "\n";
		if (balance >= 2 || balance <= -2)
		{
			std::cout << " BalanceError " << balance;
		}
		if (right)
		{
			right->printTreeInOrder();
		}
	}
	void printTreePreOrder()
	{
		std::cout << "id: " << m_id; // << "balance: " << balance << "\n";

		if (left)
		{
			left->printTreePreOrder();
		}
		if (right)
		{
			right->printTreePreOrder();
		}
	}

	void insert(T data, ID id)
	{
		if (height == -1)
		{
			m_data = data;
			m_id = id;
			height++;
			return;
		}
		if (m_id < id)
		{
			if (!right)
			{
				right = new AVLTree<T, ID>(this, data, id);
				right->insertRoll();
			}
			else
			{
				right->insert(data, id);
			}
		}
		else if (m_id == id)
		{
			throw Exist();
		}
		else
		{
			if (!left)
			{
				left = new AVLTree<T, ID>(this, data, id);
				left->insertRoll();
			}
			else
			{
				left->insert(data, id);
			}
		}
	}

	AVLTree<T, ID> *remove(ID id)
	{
		AVLTree<T, ID> *tmp = removeHelper(id);
		return tmp->getRoot();
	}
};

template <class T, class ID>
AVLTree<T, ID> *AVLTree<T, ID>::find(ID id)
{
	if (height == -1)
	{
		throw EmptyTree();
	}
	if (m_id == id)
	{
		return this;
	}
	if (m_id < id)
	{
		if (!right)
		{
			throw NotFound();
		}
		return right->find(id);
	}
	if (!left)
	{
		throw NotFound();
	}
	return left->find(id);
}

template <class T, class ID>
void AVLTree<T, ID>::swap(AVLTree<T, ID> *other)
{
	T tmpData = m_data;
	ID tmpID = m_id;
	m_data = other->m_data;
	m_id = other->m_id;
	other->m_data = tmpData;
	other->m_id = tmpID;
}

template <class T, class ID>
void AVLTree<T, ID>::updateHeightBalance()
{
	if (left && right)
	{
		height = std::max(left->height, right->height) + 1;
		balance = left->height - right->height;
	}
	else if (left)
	{
		height = left->height + 1;
		balance = height;
	}
	else if (right)
	{
		height = right->height + 1;
		balance = height * (-1);
	}
	else
	{
		height = 0;
		balance = 0;
	}
}

template <class T, class ID>
void AVLTree<T, ID>::insertRoll()
{
	updateHeightBalance();
	if (balance > -2 && balance < 2)
	{
		if (parent)
		{
			parent->insertRoll();
		}
	}
	else
	{
		if (balance == 2)
		{
			if (left->balance == 1)
			{
				rollLL();
			}
			else
			{
				rollLR();
			}
		}
		else if (balance == -2)
		{
			if (right->balance == 1)
			{
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
void AVLTree<T, ID>::rollLL()
{
	T tmpData = m_data;
	ID tmpID = m_id;
	AVLTree<T, ID> *oldA = left;
	AVLTree<T, ID> *BR = right;
	AVLTree<T, ID> *AR = oldA->right;
	AVLTree<T, ID> *AL = oldA->left;
	m_data = oldA->m_data;
	m_id = oldA->m_id;
	left = AL;
	if (AL)
	{
		AL->parent = this;
	}
	right = oldA;
	oldA->m_data = tmpData;
	oldA->m_id = tmpID;
	oldA->right = BR;
	if (BR)
	{
		BR->parent = oldA;
	}
	oldA->left = AR;
	oldA->updateHeightBalance();
	updateHeightBalance();
}

template <class T, class ID>
void AVLTree<T, ID>::rollRR()
{
	T tmpData = m_data;
	ID tmpID = m_id;
	AVLTree<T, ID> *oldA = right;
	AVLTree<T, ID> *BL = left;
	AVLTree<T, ID> *AR = oldA->right;
	AVLTree<T, ID> *AL = oldA->left;
	m_data = oldA->m_data;
	m_id = oldA->m_id;
	right = AR;
	if(AR){
		AR->parent = this;
	}
	left = oldA;
	oldA->m_data = tmpData;
	oldA->m_id = tmpID;
	oldA->left = BL;
	if (BL)
	{
		BL->parent = oldA;
	}
	oldA->right = AL;
	oldA->updateHeightBalance();
	updateHeightBalance();
}

template <class T, class ID>
void AVLTree<T, ID>::rollLR()
{
	left->rollRR();
	rollLL();
}

template <class T, class ID>
void AVLTree<T, ID>::rollRL()
{
	right->rollLL();
	rollRR();
}

template <class T, class ID>
void AVLTree<T, ID>::removeRoll()
{
	updateHeightBalance();
	if (balance == 2)
	{
		if (left->balance == 1)
		{
			rollLL();
		}
		else
		{
			rollLR();
		}
	}
	else if (balance == -2)
	{
		if (right->balance == 1)
		{
			rollRL();
		}
		else
		{
			rollRR();
		}
	}
	if (parent)
	{
		parent->removeRoll();
	}
}

template <class T, class ID>
void AVLTree<T, ID>::deleteTree(AVLTree<T, ID> *root)
{
	if (!root)
	{
		throw EmptyTree();
	}
	if (root->left)
	{
		if (!root->left->isLeaf())
		{
			deleteTree(root->left);
		}
		delete root->left;
	}
	if (root->right)
	{
		if (!root->right->isLeaf())
		{
			deleteTree(root->right);
		}
		delete root->right;
	}
	delete root;
}

template <class T, class ID>
AVLTree<T, ID> *AVLTree<T, ID>::getMostLeft()
{
	if (!left)
	{
		return this;
	}
	return left->getMostLeft();
}

#endif // AVL_TREE