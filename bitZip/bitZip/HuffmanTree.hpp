#pragma once 


#include<vector>
#include<queue>

template<class W>   //����ط���W�ͱ�ʾ���ǽ���е�Ȩֵ

struct HuffmanTreeNode
{
	//ͬʱ���ǿ��Ը������캯��
	HuffmanTreeNode(const W& weight = W())   //��Ȩֵ�����������Ȩֵû���ṩ�Ļ�
		//���Ҿ͸���һ��Ĭ�ϵ�Ȩֵ
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _weight(weight)
	{
		//���ʱ��ڵ�����ˣ���ô�������¿�
	}

	//W��ʾ���ǽڵ��е�Ȩֵ
	HuffmanTreeNode<W>* _left;   //����������
	HuffmanTreeNode<W>* _right;  //����������
	HuffmanTreeNode<W>* _parent;  //����˫��

	W _weight;
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;

	template<class W>

	struct Compare
	{
		//�������ֱȽϵ���
		bool operator()(const Node* left, const Node* right)
		{
			//���Ǹ������մ��ڵķ�ʽ�����бȽϵĲ���
			//���ǱȽϵ��ǽ�������Ȩֵ
			return left->_weight > right->_weight;
		}
	};

public:
	HuffmanTree()
		:_root(nullptr)
	{
		//�����ʱ���������Ȱ�����������һ�ſյ���
	}

	//��Ϊ���ֵĽ�㶼�����Ƕ�̬����ģ��������������֮�󣬼ǵý��пռ���ͷ�
	~HuffmanTree()
	{
		DestroyTree(_root);
	}

	//��ô���������Ӿ���Ҫ�õ�Ȩֵȥ����һ��Huffman��
	//������������Ҫ��������ֵ�����Ƿ��ضѶ���Ԫ�ؾͿ��ԣ���������ѡ�񷵻�void *
	Node* CreateHuffmanTree(const std::vector<W>& weights, const W& invalid)
		//invalid��ʾ�ľ��Ǳ����������������ʵ��һ���Ƿ���Ȩֵ
		//�Ͳ��ö��������ӵĲ�����
		//������Ҫ��Ȩֵ����ȥ����ȥ�����ǿŹ�������
	{
		//Ȼ�����ڵĵ�һ����ʵ����ȥ�����������ɭ��
		//��ÿ��ȨֵΪ����㹹�������ɭ��
		//�ö��������ݽṹȥ����
		//std::priority_queue<Node*> q;
		//�������ȼ�����Ĭ���Ǵ�ѣ�����������Ҫ����С��
		//��ô���Ǽ�ȻҪ����С�ѣ�����ֻ��Ҫ���Ƚϵķ�ʽ����һ�¸�����ʵ�Ϳ�����
		//��ô���ǾͶ����ȼ�����ȥ���и��ĵĲ���


		//��Compare()���д���

		std::priority_queue<Node*, std::vector<Node*>, Compare<W>> q;
		//������е�ʱ����������Ҫע���һ�����������Ҫ���˵��Ƿ���weights
		for (auto e : weights)
		{
			//��ȨֵΪֵȥ�������ڵ�
			if (e == invalid)
				//�Ƿ��Ķ����Ͳ��ý�����ԵĲ�����
				continue;
			q.push(new Node(e));
		}

		while (q.size() > 1)
		{
			//��ȥ��Ȩֵ��С�ĵ㣬Ȼ������ó���
			Node* left = q.top();
			q.pop();

			Node* right = q.top();
			q.pop();

			//Ȼ������ȥ����һ���µĸ��ڵ�
			Node* parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;

			//����������������˫�׽�㶼��parent
			left->_parent = parent;
			right->_parent = parent;

			q.push(parent);

		}
		//������ֻʣһ������ʱ�򣬾Ϳ��Է�����
		//��������Ҫ�õ��Ĺ���������
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
	Node* _root;  //�������һ�����ĸ��ڵ�
};


