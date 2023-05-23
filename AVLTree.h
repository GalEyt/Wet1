

#ifndef AVL_TREE
#define AVL_TREE
#include <iostream>
using namespace std;
#define COUNT 10
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
	AVLTree<T, ID> *right = nullptr;
	AVLTree<T, ID> *left = nullptr;
	AVLTree<T, ID> *parent = nullptr;
	int height = -1;
	int balance = 0;
	T m_data;
	ID m_id;

	void swap(AVLTree<T, ID> *other);
	void swapFatherRightSon();
	AVLTree<T, ID> *getMostLeft();
	void updateHeightBalance();
	void removeRoll();
	void insertRoll();
	void updateParent(AVLTree<T, ID> *newParent);
	void rollLL();
	void rollLR();
	void rollRR();
	void rollRL();
	int max(int num1, int num2);
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
		if (isLeaf())
		{
			return removeLeaf();
		}
		return removeOnlySon();
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
	AVLTree<T, ID> *find(ID id);
	AVLTree<T, ID> *getRoot()
	{

		if (!parent || height == -1)
		{
			return this;
		}
		return parent->getRoot();
	}
	AVLTree<T, ID> *getRight() { return right; }
	AVLTree<T, ID> *getLeft() { return left; }
	AVLTree<T, ID> *getParent() { return parent; }
	T getData() { return m_data; }
	ID getID() { return m_id; }
	int getHeight() { return height; }

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

	AVLTree<T, ID> *insert(T data, ID id)
	{
		insertHelper(data, id);
		return getRoot();
	}

	void insertHelper(T data, ID id)
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
void AVLTree<T, ID>::swapFatherRightSon()
{
	AVLTree<T, ID> *son = right;
	AVLTree<T, ID> *sonRight = son->right;
	AVLTree<T, ID> *tmpLeft = left;
	AVLTree<T, ID> *tmpParent = parent;
	son->updateParent(tmpParent);
	parent = son;
	son->right = this;
	son->left = tmpLeft;
	tmpLeft->updateParent(son);
	left = nullptr;
	right = sonRight;
	if (sonRight)
	{
		sonRight->updateParent(this);
	}
	updateHeightBalance();
	son->updateHeightBalance();
}

template <class T, class ID>
void AVLTree<T, ID>::swap(AVLTree<T, ID> *other)
{
	if (right == other)
	{
		swapFatherRightSon();
		return;
	}
	AVLTree<T, ID> *tmpLeftA = left;
	AVLTree<T, ID> *tmpRightA = right;
	AVLTree<T, ID> *tmpParentA = parent;
	AVLTree<T, ID> *tmpRightB = other->right;
	AVLTree<T, ID> *tmpParentB = other->parent;
	left = other->left; // should be null
	right = tmpRightB;
	if (tmpRightB)
	{
		tmpRightB->updateParent(this);
	}
	updateParent(tmpParentB);
	tmpLeftA->updateParent(other);
	tmpRightA->updateParent(other);
	other->updateParent(tmpParentA);
	updateHeightBalance();
	other->updateHeightBalance();
}

template <class T, class ID>
int AVLTree<T, ID>::max(int num1, int num2)
{
	return num1 > num2 ? num1 : num2;
}

template <class T, class ID>
void AVLTree<T, ID>::updateHeightBalance()
{
	if (left && right)
	{
		height = AVLTree::max(left->height, right->height) + 1;
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
	AVLTree<T, ID> *A = left;
	AVLTree<T, ID> *parentB = parent;
	AVLTree<T, ID> *AR = A->right;
	A->updateParent(parentB);
	updateParent(A);
	if (AR)
	{
		AR->updateParent(this);
	}
	else
	{
		left = nullptr;
	}
	updateHeightBalance();
	A->updateHeightBalance();
}

template <class T, class ID>
void AVLTree<T, ID>::rollRR()
{
	AVLTree<T, ID> *A = right;
	AVLTree<T, ID> *parentB = parent;
	AVLTree<T, ID> *AL = A->left;
	A->updateParent(parentB);
	updateParent(A);
	if (AL)
	{
		AL->updateParent(this);
	}
	else
	{
		right = nullptr;
	}
	updateHeightBalance();
	A->updateHeightBalance();
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
		if (left->balance >= 0)
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
		else
		{
			delete root->left;
		}

		root->left = nullptr;
	}
	if (root->right)
	{
		if (!root->right->isLeaf())
		{
			deleteTree(root->right);
		}
		else
		{
			delete root->right;
		}
		root->right = nullptr;
	}
	delete root;
	root = nullptr;
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

template <class T, class ID>
void AVLTree<T, ID>::updateParent(AVLTree<T, ID> *newParent)
{
	parent = newParent;
	if (!newParent)
	{
		return;
	}
	if (m_id < newParent->m_id)
	{
		newParent->left = this;
	}
	else
	{
		newParent->right = this;
	}
}

#endif // AVL_TREE