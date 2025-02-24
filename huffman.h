#pragma once
#include <cstdio>
#include <vector>
#include <algorithm> 
#include <cstring>
#include <iostream>
#include <cstdint>
#include <map>
#include "util.h"

//huffman节点
typedef struct node {
    unsigned char val;
    unsigned int freq;
    struct node* left;
    struct node* right;
}node;

struct CustomComparator {
    bool operator()(const std::string& a, const std::string& b) const {
        if (a.length() == b.length()) {
            return a < b; // 如果长度相同，按字典序排序
        }
        return a.length() < b.length(); // 主要按长度排序
    }
};

class HuffmanArchiver {
private:

    unsigned int FreqTable[256] = { 0 };
    node* root;
    std::map<unsigned char, std::string> huffmanCodes;
    std::map<std::string,unsigned char,CustomComparator> huffmanTable;
    int wpl;
    //进阶模式
    bool OffsetFlag; int offset; bool AdcInfFlag; const char* CSID; const char* CSNm; const char* CRID; const char* CRNm;
    const char* key;
    unsigned int keylen;
    bool decompressType;
    bool xorFlag;bool rc4Flag;
    int decodeType;
public:
    char adcInfo[512] = { 0 };
    uint64_t filesize = 0; //输入文件大小
    char inFileName[256];
    char outFileName[256];
    char codeFileName[256];
    HuffmanArchiver(const char* inFileName);  //作为压缩机的构造函数
    HuffmanArchiver(const char* inFileName, const char* codeFileName);//作为解压机的构造函数
    ~HuffmanArchiver();
    void generateCodes(node* root, const std::string& code);
    uint64_t getOutFileSize();//获取输出文件大小
    uint64_t getInFileSize();//获取输出文件大小
    //char* getOutContent();
    //char* getHash(char*filename);
    int getWPL() { return this->wpl; }
    bool rc4Enc();
    bool xorEnc();
    bool offsetEnc();
    bool countCharFreq();//统计字符频率  
    bool buildHuffmanTree();//建立哈夫曼树，利用了小根堆
    bool buildHuffmanCode();//建立哈夫曼编码
    bool AdcInfCheck();//附加信息效验
    bool outCodeFile();//输出指定格式的编码文件
    bool outCompressFile();//输出压缩文件
    bool outDeCompressFile();//输出解压缩文件(树形比较法)
    bool outDeCompressFile2();//输出解压缩文件(逐一比较法)
    bool readHuffmanCodeTable();
    bool compress(bool OffsetFlag, int offset, bool AdcInfFlag, const char* CSID, const char* CSNm, const char* CRID, const char* CRNm, bool xorEncFlag, bool rc4EncFlag, const char* enckey);//封装好的压缩函数
    bool decompress(bool OffsetFlag, int offset, bool AdcInfFlag, const char* CSID, const char* CSNm, const char* CRID, const char* CRNm, bool xorDecFlag, bool rc4DecFlag, const char* deckey,bool decompressType);
};