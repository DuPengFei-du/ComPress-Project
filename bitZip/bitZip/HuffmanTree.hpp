#pragma once 


#include<vector>
#include<queue>

template<class W>   //这个地方的W就表示的是结点中的权值

struct HuffmanTreeNode
{
	//同时我们可以给出构造函数
	HuffmanTreeNode(const W& weight = W())   //把权值给出来，如果权值没有提供的话
		//那我就给你一个默认的权值
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _weight(weight)
	{
		//这个时候节点就有了，那么继续向下看
	}

	//W表示的是节点中的权值
	HuffmanTreeNode<W>* _left;   //树的左子树
	HuffmanTreeNode<W>* _right;  //树的右子树
	HuffmanTreeNode<W>* _parent;  //树的双亲

	W _weight;
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;

	template<class W>

	struct Compare
	{
		//给出这种比较的类
		bool operator()(const Node* left, const Node* right)
		{
			//我们给出按照大于的方式来进行比较的操作
			//我们比较的是结点里面的权值
			return left->_weight > right->_weight;
		}
	};

public:
	HuffmanTree()
		:_root(nullptr)
	{
		//构造的时候我们首先把这棵树构造成一颗空的树
	}

	//因为树种的结点都是我们动态申请的，所以在用完成了之后，记得进行空间的释放
	~HuffmanTree()
	{
		DestroyTree(_root);
	}

	//那么，我们闲杂就需要用到权值去构建一颗Huffman树
	//我们最终是需要给出返回值的我们返回堆顶的元素就可以，所以我们选择返回void *
	Node* CreateHuffmanTree(const std::vector<W>& weights, const W& invalid)
		//invalid表示的就是遍历到的这个东西其实是一个非法的权值
		//就不用对其进行入队的操作了
		//我们需要把权值给进去才能去创建那颗哈夫曼树
	{
		//然后现在的第一步其实就是去构造二叉树的森林
		//以每个权值为根结点构造二叉树森林
		//用堆这种数据结构去创建
		//std::priority_queue<Node*> q;
		//但是优先级队列默认是大堆，但是我们需要的是小堆
		//那么我们既然要的是小堆，我们只需要将比较的方式进行一下更改其实就可以了
		//那么我们就对优先级队列去进行更改的操作


		//将Compare()进行传入

		std::priority_queue<Node*, std::vector<Node*>, Compare<W>> q;
		//在入队列的时候，我们所需要注意的一点就是我们需要过滤掉非法的weights
		for (auto e : weights)
		{
			//以权值为值去创建根节点
			if (e == invalid)
				//非法的东西就不用进行入对的操作了
				continue;
			q.push(new Node(e));
		}

		while (q.size() > 1)
		{
			//先去找权值最小的点，然后把他拿出来
			Node* left = q.top();
			q.pop();

			Node* right = q.top();
			q.pop();

			//然后现在去创建一个新的根节点
			Node* parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			//左子树和右子树的双亲结点都是parent
			left->_parent = parent;
			right->_parent = parent;

			q.push(parent);

		}
		//当堆中只剩一个结点的时候，就可以返回了
		//就是我需要用到的哈夫曼树了
		_root = q.top();
		return _root;
	}

	Node* GetRoot()
	{
		return _root;
	}

private:
	void DestroyTree(Node*& root)
	{
		if (root)
		{
			DestroyTree(root->_left);
			DestroyTree(root->_right);
			delete root;
			root = nullptr;
		}
	}
private:
	Node* _root;  //用来标记一棵树的根节点
};


