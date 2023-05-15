#ifndef NODE
#define NODE
template <class T>
class Node
{
public:
	T data;
	Node *next;

	Node();
	~Node() = default;
	Node(const Node &) = default;
	Node(const T &);
	Node &operator=(const Node &) = default;
	Node()
	{
		data = T();
		next = nullptr;
	}
	Node(const T &){
		data = T;
		next = nullptr;
	}
};

#endif // NODE