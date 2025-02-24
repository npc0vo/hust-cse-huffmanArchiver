#pragma once
#include <cstdio>
#include <vector>
#include <algorithm> 
#include <cstring>
#include <iostream>
#include <cstdint>
#include <map>
#include "util.h"

//huffman�ڵ�
typedef struct node {
    unsigned char val;
    unsigned int freq;
    struct node* left;
    struct node* right;
}node;

struct CustomComparator {
    bool operator()(const std::string& a, const std::string& b) const {
        if (a.length() == b.length()) {
            return a < b; // ���������ͬ�����ֵ�������
        }
        return a.length() < b.length(); // ��Ҫ����������
    }
};

class HuffmanArchiver {
private:

    unsigned int FreqTable[256] = { 0 };
    node* root;
    std::map<unsigned char, std::string> huffmanCodes;
    std::map<std::string,unsigned char,CustomComparator> huffmanTable;
    int wpl;
    //����ģʽ
    bool OffsetFlag; int offset; bool AdcInfFlag; const char* CSID; const char* CSNm; const char* CRID; const char* CRNm;
    const char* key;
    unsigned int keylen;
    bool decompressType;
    bool xorFlag;bool rc4Flag;
    int decodeType;
public:
    char adcInfo[512] = { 0 };
    uint64_t filesize = 0; //�����ļ���С
    char inFileName[256];
    char outFileName[256];
    char codeFileName[256];
    HuffmanArchiver(const char* inFileName);  //��Ϊѹ�����Ĺ��캯��
    HuffmanArchiver(const char* inFileName, const char* codeFileName);//��Ϊ��ѹ���Ĺ��캯��
    ~HuffmanArchiver();
    void generateCodes(node* root, const std::string& code);
    uint64_t getOutFileSize();//��ȡ����ļ���С
    uint64_t getInFileSize();//��ȡ����ļ���С
    //char* getOutContent();
    //char* getHash(char*filename);
    int getWPL() { return this->wpl; }
    bool rc4Enc();
    bool xorEnc();
    bool offsetEnc();
    bool countCharFreq();//ͳ���ַ�Ƶ��  
    bool buildHuffmanTree();//��������������������С����
    bool buildHuffmanCode();//��������������
    bool AdcInfCheck();//������ϢЧ��
    bool outCodeFile();//���ָ����ʽ�ı����ļ�
    bool outCompressFile();//���ѹ���ļ�
    bool outDeCompressFile();//�����ѹ���ļ�(���αȽϷ�)
    bool outDeCompressFile2();//�����ѹ���ļ�(��һ�ȽϷ�)
    bool readHuffmanCodeTable();
    bool compress(bool OffsetFlag, int offset, bool AdcInfFlag, const char* CSID, const char* CSNm, const char* CRID, const char* CRNm, bool xorEncFlag, bool rc4EncFlag, const char* enckey);//��װ�õ�ѹ������
    bool decompress(bool OffsetFlag, int offset, bool AdcInfFlag, const char* CSID, const char* CSNm, const char* CRID, const char* CRNm, bool xorDecFlag, bool rc4DecFlag, const char* deckey,bool decompressType);
};