#ifndef NODE
#define NODE
template <class T>
class Node
{
public:
	T data;
	Node *next;

	~Node() = default;
	Node(const Node &) = default;
	Node &operator=(const Node &) = default;
	Node()
	{
		data = T();
		next = nullptr;
	}
	Node(const T & newData){
		data = newData;
		next = nullptr;
	}
};

#endif // NODE