#ifndef AVL_TREE
#define AVL_TREE

class Exist{};

template <class T>
class AVLTree
{
private:
    AVLTree* right;
    AVLTree* left;
    AVLTree* parent;
    int height;
    int balance;
    T m_data;

    

public:
    AVLTree() : right(nullptr), left(nullptr), parent(nullptr), height(-1), balance(0), m_data(nullptr) {}
    AVLTree(AVLTree* parent, T data) : right(nullptr), left(nullptr), parent(parent), height(0), balance(0), m_data(data) {}
    
    ~AVLTree() {
        //loop and delete all
    }
    void insertRoll{}
    void insert(T data){
        if(!m_data){
            m_data = data;
            height++;
        }
        if(m_data < data){
            if(!right){
                right = AVLTree(this, data);
                right->insertRoll();
            }else{
                right->insert(data);
            }
        }else if(m_data == data){
            throw Exist();
        }
        else{
            if(!left){
                left = AVLTree(this, data);
                left->insertRoll();
            }else{
                left->insert(data);
            }
        }
    }

    void remove()

};


#endif // AVL_TREE