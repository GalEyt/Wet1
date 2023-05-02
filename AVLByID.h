#ifndef AVL_TREE
#define AVL_TREE

class Exist {};
class EmptyTree {};
class NotFound {};

template <class T>
class AVLTreeByID
{
private:
	AVLTreeByID<T>* right;
	AVLTreeByID<T>* left;
	AVLTreeByID<T>* parent;
	int height;
	int balance;
	T m_data;
	int m_id;

	AVLTreeByID<T>* swap(AVLTreeByID<T>* first, AVLTreeByID<T>* second);
	AVLTreeByID<T>* find(int id);
	AVLTreeByID<T>* getRAL(AVLTreeByID<T>* node); // Right and then all left
	void removeRoll();
	void insertRoll();

public:
	AVLTreeByID() : right(nullptr), left(nullptr), parent(nullptr), height(-1), balance(0), m_data(nullptr), m_id(0) {}
	AVLTreeByID(AVLTreeByID<T>* parent, T data, id) : right(nullptr), left(nullptr), parent(parent), height(0), balance(0), m_data(data), m_id(id) {}

	~AVLTreeByID() {
		//loop and delete all
	}

	

	void insert(T data, int id) {
		if (!m_data) {
			m_data = data;
			height++;
		}
		if (m_data < data) {
			if (!right) {
				right = new AVLTreeByID<T>(this, data, id);
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
				left = new AVLTreeByID<T>(this, data, id);
				left->insertRoll();
			}
			else {
				left->insert(data);
			}
		}
	}



	void remove(int id) {
		AVLTreeByID<T>* toDelete = find(id);
		if (toDelte->right && toDelte->left) {
			AVLTreeByID<T>* toSwich = getRAL(toDelete);
			swap(toDelete, toSwich);
		}
		else if (toDelete->left) {
			swap(toDelete, toDelete->left);
		}
		else if (toDelete->right) {
			swap(toDelete, toDelete->right);
		}
		else if (!toDelete->parent) {
			~AVLTreeByID();
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
		AVLTreeByID<T>* parent = toDelete->parent;
		delete toDelete;
		parent->removeRoll();

	}

};

template <class T>
AVLTreeByID<T>* AVLTreeByID<T>::find(int id) {
	if (!m_data) {
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
}

#endif // AVL_TREE