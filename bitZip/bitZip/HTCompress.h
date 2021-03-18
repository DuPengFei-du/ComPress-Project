#pragma once 

#include<string>
#include<vector>
#include"HuffmanTree.hpp"

using namespace std;

typedef unsigned long long ulg;   //因为字符出现的次数可能很多
//而且我也确定他到底出现多少次，所以我给成ulg类型

typedef unsigned char uch;
struct CharInfo
{
	//char _ch;  //我们要去统计ch这个字符一共出现了多少次
	//一开始像上面这样定义出现的次数的话，其实是会崩溃的，因为char所能表示的类型
	//范围为:-128~+127,所以在有些情况下像上面这个样子其实是会发生崩溃的问题的
	uch _ch;

	ulg _appearCount;  //代表出现的次数
	std::string _strCode;//每一个字符最后还会有他自己的编码


	//添加响应的构造函数
	CharInfo(ulg appCount = 0)
		:_appearCount(appCount)
	{

	}

	CharInfo operator+(const CharInfo& c)
	{
		//我们要的是权值进行相加，所以我们对出现的次数进行重载的操作
		return CharInfo(_appearCount + c._appearCount);
	}

	//用权值来进行比较的操作
	bool operator>(const CharInfo& c)const
	{
		return _appearCount > c._appearCount;
	}

	bool operator==(const CharInfo& c)const
	{
		//用出现的次数来比较两个东西是不是相同的就可以了
		return _appearCount == c._appearCount;
	}

	bool operator!=(const CharInfo& c)const
	{
		//用出现的次数来比较两个东西是不是相同的就可以了
		return _appearCount != c._appearCount;
	}
};


class HTCompress
{
public:

	HTCompress();
	//压缩一个文件和解压缩一个文件的时候，我都需要把文件的路径给他传进来
	void CompressFile(const std::string& filePath);

	void UnCompressFile(const std::string& filePath);

	//写入头部信息
	void WriteHeadInfo(FILE* fOut, const std::string& filePath);

private:
	//我们需要根据huffman树去生成最终的编码
	void GeneteCode(HuffmanTreeNode<CharInfo>* root);   //把结点的值传入

	//我们自己给出一个读取每一行文件内容的函数其实就可以了
	//从fIn中进行读取的操作，把读取到的内容放在第二个参数里面就可以了
	void GetLine(FILE* fIn, std::string& s);

private:
	//数据在文件中都是以字节的方式来进行保存的
	//一个字节占有的比特位是8个比特位，可以表示256个字符
	//所以我们vector的空间给成256就可以了
	//将字符的信息保存到结构体里面就可以了
	std::vector<CharInfo> _charInfo;
};
