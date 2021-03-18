#pragma once 

#include<string>
#include<vector>
#include"HuffmanTree.hpp"

using namespace std;

typedef unsigned long long ulg;   //��Ϊ�ַ����ֵĴ������ܺܶ�
//������Ҳȷ�������׳��ֶ��ٴΣ������Ҹ���ulg����

typedef unsigned char uch;
struct CharInfo
{
	//char _ch;  //����Ҫȥͳ��ch����ַ�һ�������˶��ٴ�
	//һ��ʼ����������������ֵĴ����Ļ�����ʵ�ǻ�����ģ���Ϊchar���ܱ�ʾ������
	//��ΧΪ:-128~+127,��������Щ��������������������ʵ�ǻᷢ�������������
	uch _ch;

	ulg _appearCount;  //������ֵĴ���
	std::string _strCode;//ÿһ���ַ���󻹻������Լ��ı���


	//�����Ӧ�Ĺ��캯��
	CharInfo(ulg appCount = 0)
		:_appearCount(appCount)
	{

	}

	CharInfo operator+(const CharInfo& c)
	{
		//����Ҫ����Ȩֵ������ӣ��������ǶԳ��ֵĴ����������صĲ���
		return CharInfo(_appearCount + c._appearCount);
	}

	//��Ȩֵ�����бȽϵĲ���
	bool operator>(const CharInfo& c)const
	{
		return _appearCount > c._appearCount;
	}

	bool operator==(const CharInfo& c)const
	{
		//�ó��ֵĴ������Ƚ����������ǲ�����ͬ�ľͿ�����
		return _appearCount == c._appearCount;
	}

	bool operator!=(const CharInfo& c)const
	{
		//�ó��ֵĴ������Ƚ����������ǲ�����ͬ�ľͿ�����
		return _appearCount != c._appearCount;
	}
};


class HTCompress
{
public:

	HTCompress();
	//ѹ��һ���ļ��ͽ�ѹ��һ���ļ���ʱ���Ҷ���Ҫ���ļ���·������������
	void CompressFile(const std::string& filePath);

	void UnCompressFile(const std::string& filePath);

	//д��ͷ����Ϣ
	void WriteHeadInfo(FILE* fOut, const std::string& filePath);

private:
	//������Ҫ����huffman��ȥ�������յı���
	void GeneteCode(HuffmanTreeNode<CharInfo>* root);   //�ѽ���ֵ����

	//�����Լ�����һ����ȡÿһ���ļ����ݵĺ�����ʵ�Ϳ�����
	//��fIn�н��ж�ȡ�Ĳ������Ѷ�ȡ�������ݷ��ڵڶ�����������Ϳ�����
	void GetLine(FILE* fIn, std::string& s);

private:
	//�������ļ��ж������ֽڵķ�ʽ�����б����
	//һ���ֽ�ռ�еı���λ��8������λ�����Ա�ʾ256���ַ�
	//��������vector�Ŀռ����256�Ϳ�����
	//���ַ�����Ϣ���浽�ṹ������Ϳ�����
	std::vector<CharInfo> _charInfo;
};
