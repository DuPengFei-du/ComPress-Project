#include"HTCompress.h"
#include<iostream>
using namespace std;

HTCompress::HTCompress()
{
	//给出构造函数
	_charInfo.resize(256);
	for (size_t i = 0; i < 256; ++i)
	{
		_charInfo[i]._ch = i;
		//一个字符到底出现了多少次，我现在是不知道的的
		//我只知道第i个字符他就是第i个字符
		//把出现的次数我们一开始给成0
		_charInfo[i]._appearCount = 0;
	}
}

void HTCompress::CompressFile(const std::string& filePath)
{
	//然后就要去统计文件中每个字符出现的次数
	//那么我既然要去统计这文件中所需要字符的个数，首先我就需要把这个文件打开才可以
	FILE* fIn = fopen(filePath.c_str(), "rb");  //但是fopen只接受C语言格式的字符串
	//那也没关系我们使用c_str就会把其转化成为C语言格式的字符串了
	//同时我们以只读的方式将这个文件进行打开的操作
	if (nullptr == fIn)
	{
		//说明打开文件失败了
		cout << "待压缩文件路径出错" << endl;
		return;
	}

	//打开成功的话，就去统计文件中每个字符出现的次数，然后还需要保存起来
	//那么，我既然要去统计文件中每个字符出现的次数的话
	//我就需要去读文件
	uch readBuff[1024];
	while (true)
	{
		//因为我并不知道文件有多大，所以我不知道我对这个文件需要进行几次的读操作
		//索性我直接给成循环去进行读了
		size_t rdsize = fread(readBuff, 1, 1024, fIn);
		//fread的返回值是成功读取到的元素的个数
		if (rdsize == 0)
			//说明我这次一个字节都没有读取到
			//也说明我已经读取到文件的末尾了
			break;
		//如果不是0，那么说明文件中其实还是有内容可以读出来的
		for (size_t i = 0; i < rdsize; ++i)
		{
			//那么，如何进行统计呢?我们就以这个字符的ASCII码作为下标来对其进行加一的操作
			//以字符的ASCII码作为下标来进行统计的操作
			_charInfo[readBuff[i]]._appearCount++;
		}

	}

	//2.然后以ChInfo作为权值去创建huffman树
	HuffmanTree<CharInfo> ht;
	ht.CreateHuffmanTree(_charInfo, CharInfo(0));
	//出现次数为0次的字符，我们就将其视为非法的字符
	//不对其进行入队的操作

	//3.然后再获取每个字符所对应的哈夫曼编码
	GeneteCode(ht.GetRoot());

	//获取到了编码之后
	//4.我们需要用获取到的编码队源文件中的每个字符进行重新的改写
	//然后这个时候我们需要再次去读取一遍文件

	//但是我们在之前遍历文件的内容的时候，我们已经遍了了一次文件了,那我们既然已经
	//遍历了一次文件了，那么我们的文件指针就已经处于文件的末尾了
	//所以我们在进行第二次遍历的时候，我们需要对文件指针进行重新赋值的操作

	//将文件指针恢复到起始得位置
	rewind(fIn);

	//给出压缩文件
	FILE* fOut = fopen("compressResult.txt", "wb");   //这个文件是用来写压缩结果的

	WriteHeadInfo(fOut, filePath);   //文件的路径也需要传入

	uch chData = 0;   //代表的是字节，一个字节最多只能放置8个比特位

	uch bitCount = 0;
	while (true)
	{
		size_t rdSize = fread(readBuff, 1, 1024, fIn);
		//fread返回的实际读到的字符的个数
		if (rdSize == 0)   //如果是0的话，说明我们已经处理完了
			break;
		for (size_t i = 0; i < rdSize; ++i)
		{
			//然后我们现在要去找到结点对应的编码
			string& strCode = _charInfo[readBuff[i]]._strCode;

			//将该编码的每个二进制的比特位放置到一个字节里面
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				//左移一位就是给他的右边补了一个0
				chData <<= 1;

				//编码的比特位可能是0也可能是1
				if (strCode[j] == '1')
				{
					chData |= 1;    //这一步的操作其实就相当于是把第i个比特位放到了chData里面
				}
				//如果是0的话，那么只需要进行左移的操作，就不用进行或操作
				//每次放进去一个字节，就要把count++
				bitCount++;
				if (8 == bitCount)
				{
					//当8个比特位放满的时候，就说明第一个字节已经填充好了
					//然后我们将该字节写入到压缩文件那就可以了
					fputc(chData, fOut);
					//既然我已经完成写入了，我把count给成0就ok了
					bitCount = 0;
					chData = 0;
					//那既然我要写入到压缩文件，那么我就需要把压缩文件给出来
				}
			}
		}
	}

	//出了循环之后，如果chData中有效比特位不够8个的时候，是没有写入到压缩文件当中的

	if (bitCount > 0 && bitCount < 8)   //那么就说明最后的那个字节其实不够8个比特位的
	{
		//其实在这种情况也是很好处理的，我们只需要对其进行一次单独的处理就可以了
		chData <<= (8 - bitCount);
		fputc(chData, fOut);
		//但是特殊处理的时候还需要注意一个情况，就是说我们在进行解压缩的时候
		//我们是从左往右依次进行解压缩的操作的
		//最后一个字节只有低7个比特位对我们来说是有效的
		//那既然高位的哪一位是无效的，那么我们就把最后一个字节向左移动一位
		//移动完成之后，最后的一个比特位不去进行解压缩就可以了
	}
	//最后的时候，需要把文件关掉
	fclose(fIn);
	fclose(fOut);

}


//先走到叶子节点得位置，然后顺着叶子结点的位置向上进行编码的获取
void HTCompress::GeneteCode(HuffmanTreeNode<CharInfo>* root)
{
	if (root == nullptr)
		return;

	//通过递归的方式让代码不断地向下走
	GeneteCode(root->_left);
	GeneteCode(root->_right);

	if (root->_left == nullptr && root->_right == nullptr)
	{
		//就说明走到了叶子节点了
		HuffmanTreeNode<CharInfo>* cur = root;
		//同时还要去找到这个节点的双亲结点
		HuffmanTreeNode<CharInfo>* parent = cur->_parent;

		//用这个来保存我们的编码
		string& strCode = _charInfo[cur->_weight._ch]._strCode;

		while (parent)
		{
			if (cur == parent->_left)
				strCode += '0';
			else
				strCode += '1';
			cur = parent;
			parent = cur->_parent;
		}
		reverse(strCode.begin(), strCode.end());
	}
}

//调用的实际在我们写入压缩数据之间，调用这个方法
void HTCompress::WriteHeadInfo(FILE* fOut, const string& filePath)
//把原文件的路径传进去是为了得到原文件的后缀的名称
{
	//文件中所需要包含的信息
	//1.原文件的后缀
	string filePostFix = filePath.substr(filePath.find('.'));   //这一行是为了得到原文件后缀的名称
	//得到后缀名之后，我们需要把后缀名写入到文件当中
	filePostFix += '\n';   //一次写一行的内容
	//2.字节，出现次数的总行数
	size_t szCount = 0;   //因为我们一开始也不知道他到底出现了多少次
	//szCount表示行的次数
	//所以我在一开始的时候把出现的次数给成1

	//3.字节，出现次数的信息---每条信息占有一行的内容
	string chAppear;
	for (size_t i = 0; i < 256; ++i)
	{
		if (0 != _charInfo[i]._appearCount)
		{
			chAppear += _charInfo[i]._ch;
			chAppear += ",";   //逗号前面是字符，逗号后面是出现的次数
			chAppear += to_string(_charInfo[i]._appearCount);   //之所以使用to_stirng
			//是因为两个相加的变量的类型是不一样的，所以需要进行强制类型转化的操作
			//然后又因为每个内容需要独立的占有一行，所以加上一个\n其实就是可以的了
			chAppear += "\n";
			szCount++;
		}
	}
	//为什么每条内容需要占一行，因为每条内容占一行的话，我们在读取的时候就会方便一些
	//我们只需要去读取那么多行其实就是可以的了
	//(上面是在真正写压缩数据之前需要具备的信息)

	//当这些压缩信息全部都有了的时候，我们只需要把这些内容进行一个写入的操作其实就是可以的了
	//先去写文件的后缀名
	fwrite(filePostFix.c_str(), 1, filePostFix.size(), fOut);

	//然后写入行数
	string strCount;
	//把strCount转化成字符串的格式就可以了
	strCount = to_string(szCount);
	strCount += "\n";
	fwrite(strCount.c_str(), 1, strCount.size(), fOut);

	//最后写入每一个字符出现的次数
	fwrite(chAppear.c_str(), 1, chAppear.size(), fOut);

}

void HTCompress::UnCompressFile(const std::string& filePath)
{
	//现在要去进行解压缩的话，首先我们一进来就需要把这个文件给打开
	FILE* fIn = fopen(filePath.c_str(), "rb");
	if (fIn == nullptr)
	{
		//如果文件为空的话，那么就说明其实是有问题的
		cout << "压缩文件的路径是有问题的" << endl;
		return;
	}

	//当然，我们不仅要保存原文件的路径我还要保存原文件的名称
	//否则的话，压缩出来的文件的名称其实可能就不是那么符合我们的想法了
	//那我们这个时候还是把名字直接写固定，然后文件的后缀名从压缩文件中获取
	string filename("2");
	//有了文件的名称之后，我现在需要去获取文件的后缀名
	string s;
	GetLine(fIn, s);
	filename += s;
	//这里读取结束之后，我们就相当于是获取到了我们解压缩的文件
	//解压缩文件的文件名称就是2.txt

	//然后我现在要去获取字节次数总的行数
	s = " ";  //首先我需要将文件s进行清空的操作，然后从头进行读取的操作
	GetLine(fIn, s);
	size_t szCount = atoi(s.c_str());

	//读取字节次数的每行信息
	for (size_t i = 0; i < szCount; ++i)
	{
		s = "";

		//注意，我们在这里需要对换行的符号来单独的进行处理
		GetLine(fIn, s);
		//如果你读取了一行之后，s里面的东西仍然是空的话，那么就说明
		//我们读到的是一个换行，那么我们就把\n加进去
		if ("" == s)
		{
			s += "\n";
			//加上去之后，我再次去进行一次读的操作
			GetLine(fIn, s);

		}

		//因为字符实在第一个位置，然后字符的后面还有一个逗号
		//然后逗号之后才是字符出现的次数，所以需要+2才可以真正获取到字符出现的次数
		_charInfo[(uch)s[0]]._appearCount = atoi(s.c_str() + 2);
	}

	//既然我们现在需要的关于huffman树的信息我们已经全部获取的到了，那么我们现在其实就只是
	//需要去还原我们之前的那一棵huffman树就可以了
	HuffmanTree<CharInfo> ht;

	ht.CreateHuffmanTree(_charInfo, CharInfo(0));  //去除掉出现次数为0的字符
	//出现次数为0的字符就不要存在在huffman树中了
	//然后现在huffman树其实也有了

	//那么现在我们就需要去进行解压缩的操作了
	//当然我们在解压缩成功之后，我们同样需要把我们解压缩之后的结果
	//写入到一个新的字符串当中去
	FILE* fOut = fopen(filename.c_str(), "wb");  //我们要以写的方式将这个文件进行打开的操作
	uch readBuff[1024];   //我们将读取到的内容写入到这个空间当中

	//给出huffman树
	//让其从根节点的位置进行遍历的操作
	HuffmanTreeNode<CharInfo>* cur = ht.GetRoot();

	uch bitCount = 8;   //一开始有8个比特位
	uch chBit;   //一开始有8个比特位
	ulg fileSize = cur->_weight._appearCount;   //把文件的大小拿过来
	//这样我在进行解压的时候，我就知道我该去解压缩多少的大小了

	while (true)
	{
		size_t rdSize = fread(readBuff, 1, 1024, fIn);
		if (0 == rdSize)
			break;   //如果等于0的话，就说明我们已经读取到了文件的末尾了
		//就不会再继续去进行读的操作了

		//然后我们一个字节一个字节来进行解压缩的操作
		//将rdBuff中每个字节逐比特位的来进行解析的操作
		for (size_t i = 0; i < rdSize; ++i)
		{
			bitCount = 8;
			chBit = readBuff[i];
			//然后我们需要根据信息去遍历Huffman树，那么我们既然要去遍历huffman树的话
			//那我们肯定是要给出Huffman树的
			//将readbuff[i]中保存字节的八个比特为从高到低的来逐个进行检测
			while (bitCount)
			{
				if (chBit & 0x80)
				{
					//这一步其实就是去检测一个字节中最高位的那个比特位到底是不是0
					//如果是非0的数据的话，那么就说明这个比特的高位其实是1
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}
				if (cur->_left == nullptr && cur->_right == nullptr)
				{
					//就说明cur现在在叶子结点的位置
					//那么就说明我们已经解压缩出来了一个字符了
					//既然这个字符已经解压缩出来了，那么我们需要把这个字符
					//写道我们的解压缩文件当中去
					fputc(cur->_weight._ch, fOut);    //像fOut中去写
					//然后既然得到了一个字符之后我们就需要让cur回到根节点的位置
					cur = ht.GetRoot();

					//然后我们每次解压缩出来一个字节就对其进行--的操作

					//这一小部分主要是为了处理最后一字节不够8个比特位的情况
					fileSize--;
					if (0 == fileSize)
						break;
				}
				bitCount--;
				//当8个比特位处理完成的时候我们就需要重新开始
				//去读取一个字节
				chBit <<= 1;   //然后朝左边去移动一位
			}
		}
	}
	fclose(fIn);
	fclose(fOut);

}

void HTCompress::GetLine(FILE* fIn, std::string& s)
{
	uch ch;
	//feof用来判断文件指针是不是在文件的末尾
	while (!feof(fIn))
	{
		ch = fgetc(fIn);     //fget是用来读取一个字符的
		if ('\n' == ch)
			break;    //如果字符是换行符号的话，那么就说明我这一行的内容就已经读取完毕了
		//那么我们就跳出循环
		//如果读到的不是换行符号的话，那么我们只需要把他加到文件中就可以了
		s += ch;
	}
}