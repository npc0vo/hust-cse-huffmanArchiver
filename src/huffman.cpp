#include "pch.h"
#include "huffman.h"
#include "HuffmanArchiverDlg.h"
#include "MinHeap.h"
HuffmanArchiver::HuffmanArchiver(const char* inFileName)
{
    // ��ʼ�������ļ���
    strncpy(this->inFileName, inFileName, sizeof(this->inFileName) - 1);
    this->inFileName[sizeof(this->inFileName) - 1] = '\0';

    //��ʼ��codefile���ļ���
    // ��ȡ�����ļ���Ŀ¼·��
    const char* lastSlash = strrchr(inFileName, '\\');
    if (lastSlash) {
        size_t len = lastSlash - inFileName + 1;
        strncpy(this->codeFileName, inFileName, len);
        this->codeFileName[len] = '\0';
    }
    else {
        this->codeFileName[0] = '\0'; // ���û��Ŀ¼·����ֱ��ʹ�õ�ǰĿ¼
    }
    strcat(this->codeFileName, "code.txt");

    // ��������ļ��� ��.txt��Ϊ.hfm
    const char* dot = strrchr(inFileName, '.');
    if (dot) {
        size_t len = dot - inFileName;
        strncpy(this->outFileName, inFileName, len);
        this->outFileName[len] = '\0';
    }
    else {
        strncpy(this->outFileName, inFileName, sizeof(this->outFileName) - 1);
        this->outFileName[sizeof(this->outFileName) - 1] = '\0';
    }
    strcat(this->outFileName, ".hfm");
    // ��ʼ���ļ���СΪ0
    //std::cout<<this->outFileName<<std::endl;
    filesize = 0;

    // ��ʼ�����������ͱ����ָ��Ϊnullptr
    root = nullptr;
}

HuffmanArchiver::HuffmanArchiver(const char* inFileName, const char* codeFileName)
{
    // ��ʼ�������ļ���
    strncpy(this->inFileName, inFileName, sizeof(this->inFileName) - 1);
    this->inFileName[sizeof(this->inFileName) - 1] = '\0';

    // ��ʼ�������ļ���
    
    strncpy(this->codeFileName, codeFileName, sizeof(this->codeFileName) - 1);
    // ��������ļ��� ��.hfm��Ϊ_j.txt
    const char* dot = strrchr(inFileName, '.');
    if (dot) {
        size_t len = dot - inFileName;
        strncpy(this->outFileName, inFileName, len);
        this->outFileName[len] = '\0';
    }
    else {
        strncpy(this->outFileName, inFileName, sizeof(this->outFileName) - 1);
        this->outFileName[sizeof(this->outFileName) - 1] = '\0';
    }
    strcat(this->outFileName, "_j.txt");

    // ��ʼ���ļ���СΪ0
    filesize = 0;

    // ��ʼ�����������ͱ����ָ��Ϊnullptr
    root = nullptr;
}

HuffmanArchiver::~HuffmanArchiver()
{
    //TODO:�ͷŹ��������ͱ����

}
uint64_t HuffmanArchiver::getOutFileSize()
{
    FILE* file = fopen(this->outFileName, "rb"); // �Զ�����ֻ��ģʽ���ļ�
    if (file == NULL)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::getOutFileSize():failed to open outFile");
        return -1; // ���� -1 ��ʾ�ļ���ʧ��
    }

    // ���ļ�ָ���ƶ����ļ�ĩβ
    fseek(file, 0, SEEK_END);

    // ��ȡ�ļ���С
    long long fileSize = ftell(file);

    // �ر��ļ�
    fclose(file);

    return fileSize;
}
uint64_t HuffmanArchiver::getInFileSize()
{
    FILE* file = fopen(this->inFileName, "rb"); // �Զ�����ֻ��ģʽ���ļ�
    if (file == NULL)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::getOutFileSize():failed to open outFile");
        return -1; // ���� -1 ��ʾ�ļ���ʧ��
    }

    // ���ļ�ָ���ƶ����ļ�ĩβ
    fseek(file, 0, SEEK_END);

    // ��ȡ�ļ���С
    long long fileSize = ftell(file);

    // �ر��ļ�
    fclose(file);

    return fileSize;
}
bool HuffmanArchiver::rc4Enc()
{
    FILE* fp = fopen(this->inFileName, "rb");
    if (!fp)
    {
        return false;
    }
    unsigned char ch;
    //��ʼ��RC4

    RC4State rc4state;
    rc4_init(&rc4state, this->key, this->keylen);
    //����и�����Ϣ
    if (AdcInfFlag)
    {
        //��ȡ������Ϣ
        for (int i = 0; i < strlen(CSID); i++)
        {
            ch = CSID[i];
            ch=rc4_encrypt_byte(&rc4state, ch);
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = ',';
        ch = rc4_encrypt_byte(&rc4state, ch);
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CSNm); i++)
        {
            ch = CSNm[i];
            ch = rc4_encrypt_byte(&rc4state, ch);
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = '\n';
        ch = rc4_encrypt_byte(&rc4state, ch);
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CRID); i++)
        {
            ch = CRID[i];
            ch = rc4_encrypt_byte(&rc4state, ch);
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = ',';
        ch = rc4_encrypt_byte(&rc4state, ch);
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CRNm); i++)
        {
            ch = CRNm[i];
            ch = rc4_encrypt_byte(&rc4state, ch);
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = '\n';
        ch = rc4_encrypt_byte(&rc4state, ch);
        this->FreqTable[ch]++;
        this->filesize++;
    }
    while (fread(&ch, sizeof(unsigned char), 1, fp) == 1)
    {

        ch = rc4_encrypt_byte(&rc4state, ch);  //228->
        this->FreqTable[ch]++;
        this->filesize++;
    }

    return true;
}
bool HuffmanArchiver::offsetEnc()
{
    FILE* fp = fopen(this->inFileName, "rb");
    if (!fp)
    {
        return false;
    }
    unsigned char ch;
    //����и�����Ϣ
    if (AdcInfFlag)
    {
        //��ȡ������Ϣ
        for (int i = 0; i < strlen(CSID); i++)
        {
            ch = CSID[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = ',';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CSNm); i++)
        {
            ch = CSNm[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = '\n';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CRID); i++)
        {
            ch = CRID[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = ',';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CRNm); i++)
        {
            ch = CRNm[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = '\n';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;

        }
        this->FreqTable[ch]++;
        this->filesize++;
    }
    while (fread(&ch, sizeof(unsigned char), 1, fp) == 1)
    {

        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        this->FreqTable[ch]++;
        this->filesize++;
    }

    //Debug
     for (int i = 0; i < 256; i++) {
         if (this->FreqTable[i])
         {
            mylog(LogPath,"0x%02x:%d\n", i, this->FreqTable[i]);
         }
     }
    return true;
}
bool HuffmanArchiver::xorEnc()
{
    FILE* fp = fopen(this->inFileName, "rb");
    if (!fp)
    {
        return false;
    }
    long long index = 0;
    unsigned char ch;
    //����и�����Ϣ
    if (AdcInfFlag)
    {
        //��ȡ������Ϣ
        for (int i = 0; i < strlen(CSID); i++)
        {
            ch = CSID[i];
            ch = ch ^ key[index % keylen];
            index++;
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = ',';
        ch = ch ^ key[index % keylen];
        index++;
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CSNm); i++)
        {
            ch = CSNm[i];
            ch = ch ^ key[index % keylen];
            index++;
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = '\n';
        ch = ch ^ key[index % keylen];
        index++;
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CRID); i++)
        {
            ch = CRID[i];
            ch = ch ^ key[index % keylen];
            index++;
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = ',';
        ch = ch ^ key[index % keylen];
        index++;
        this->FreqTable[ch]++;
        this->filesize++;
        for (int i = 0; i < strlen(CRNm); i++)
        {
            ch = CRNm[i];
            ch = ch ^ key[index % keylen];
            index++;
            this->FreqTable[ch]++;
            this->filesize++;
        }
        ch = '\n';
        ch = ch ^ key[index % keylen];
        index++;
        this->FreqTable[ch]++;
        this->filesize++;
    }
    while (fread(&ch, sizeof(unsigned char), 1, fp) == 1)
    {
        ch = ch ^ key[index % keylen];
        index++;
        this->FreqTable[ch]++;
        this->filesize++;
    }

    //����Ƶ�ʱ�
    for (int i = 0; i < 256; i++) {
        if (this->FreqTable[i])
        {
            mylog(LogPath, "0x%02x:%d\n", i, this->FreqTable[i]);
        }
    }
    return true;
}
//bool HuffmanArchiver::rc4Enc()
//{
//    return true;
//}
bool HuffmanArchiver::countCharFreq()
{
    //��Ϊ�����ֽڶ�ȡ���Ҵ���ģ���������ʱ�ļ��������ڼ��ܵ�ͬʱ�ͼ����ÿ���ַ���Ƶ��
    if (this->xorFlag)
    {
        return xorEnc();
    }
    else if (this->rc4Flag)
    {
        return rc4Enc();
    }
    else
    {
        return offsetEnc(); //Ĭ��ʹ��ƫ�Ƽ��ܰ��������ܵ�����
    }
}

void printHuffmanTree(node* root, int depth = 0, char prefix = ' ') {
    if (root == nullptr) {
        return;
    }

    // ��ӡ��ǰ�ڵ���Ϣ
    for (int i = 0; i < depth; ++i) {
        mylog(LogPath,"  ");
    }
    mylog(LogPath,"%c", prefix);
    if (root->val == 0) {
        mylog(LogPath, "Internal Node, Frequency: %u\n", root->freq);
    }
    else {
        mylog(LogPath, "Character: %x, Frequency: %u\n", root->val, root->freq);
    }

    // �ݹ��ӡ��������������
    printHuffmanTree(root->left, depth + 1, 'L');
    printHuffmanTree(root->right, depth + 1, 'R');
}

void HuffmanArchiver::generateCodes(node* root, const std::string& code) {
    if (root == nullptr) {
        return;
    }

    // �����Ҷ�ӽڵ㣬�������
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->val] = code;
        this->wpl += root->freq * code.length();
    }

    // �ݹ��������������������ı���
    generateCodes(root->left, code + "0");
    generateCodes(root->right, code + "1");
}

bool HuffmanArchiver::buildHuffmanTree() {
    // ������С�Ѳ���ʼ��Ҷ�ӽڵ�
    MinHeap minHeap;
    for (int i = 0; i < 256; ++i) {
        if (FreqTable[i] > 0) {
            node* newNode = new node;
            newNode->val = static_cast<unsigned char>(i);
            newNode->freq = FreqTable[i];
            newNode->left = newNode->right = nullptr;
            minHeap.insert(newNode);
        }
    }

    // ����������������ļ���ֻ��һ���ַ�
    if (minHeap.size() == 0) {
        root = nullptr;
        return true;
    }

    // ����Huffman��
    while (minHeap.size() > 1) {
        node* left = minHeap.extractMin();
        node* right = minHeap.extractMin();

        node* internalNode = new node;
        internalNode->freq = left->freq + right->freq;
        internalNode->val =max(left->val, right->val);  // ȡ�ӽڵ���ASCII��ϴ���ַ���Ϊ�ڲ��ڵ���ַ�
        internalNode->left = left;
        internalNode->right = right;

        minHeap.insert(internalNode);
    }

    // ���ʣ�µĽڵ���Ǹ��ڵ�
    root = minHeap.extractMin();

    //DEBUG
    // printf("Huffman Tree:\n");
    // printHuffmanTree(root);
    return true;
}

bool HuffmanArchiver::buildHuffmanCode() {
    if (root == nullptr) {
        return false;
    }

    // �Ӹ��ڵ㿪ʼ���ɱ���
    generateCodes(root, "");

    //DEBUG
    // printf("FileSize:%d",filesize);
    // printf("Huffman Codes:\n");
    // for (const auto& pair : huffmanCodes) {
    //     printf("hex: 0x%x, Code: %s\n", pair.first, pair.second.c_str());
    // }

    return true;
}
bool HuffmanArchiver::outCodeFile()
{
    FILE* codeFile = fopen(this->codeFileName, "w");
    if (!codeFile)
    {
        return false;
    }
    //�������filesize
    fprintf(codeFile, "%u\n", this->filesize);

    //Ȼ�����ÿ���ַ��ı���
    for (const auto& pair : huffmanCodes)
    {
        //printf("0x%02x:%s\n", pair.first, pair.second.c_str());
        int len = pair.second.length();
        fprintf(codeFile, "0x%02X 0x%02X ", pair.first, len);
        int index = 0;
        while (len > 0)
        {
            //��������codeתΪhex ����8λ�Ĳ�0
            unsigned char byte = 0;
            for (int i = 0; i < 8 && index < pair.second.length(); ++i, ++index)
            {
                byte <<= 1;
                if (pair.second[index] == '1')
                {
                    byte |= 1;
                }
            }

            if (len <= 8)
            {
                byte <<= 8 - len;
            }

            // Print the byte
            if (len <= 8)
            {
                fprintf(codeFile, "0x%02X\n", byte);
            }
            else
            {
                fprintf(codeFile, "0x%02X ", byte);
            }
            len -= 8;
        }
    }
    fclose(codeFile);
    return true;
}
bool HuffmanArchiver::outCompressFile()
{
    FILE* fp = fopen(this->inFileName, "rb");
    if (!fp)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::outCompressFile:open infile failed");
        return false;
    }
    FILE* out = fopen(this->outFileName, "wb");
    if (!out)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::outCompressFile:open outfile failed");
        fclose(fp);
        return false;
    }

    unsigned char ch;
    long long index = 0;
    std::string codeStr;
    //��̬���ӵ��շ�����Ϣ

    RC4State state;
    if (rc4Flag)
    {
        rc4_init(&state, this->key, this->keylen);
    }
    if (AdcInfFlag)
    {
        for (int i = 0; i < strlen(CSID); i++)
        {
            ch = CSID[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            else if (xorFlag)
            {
                ch = (ch ^ key[index % keylen]);
            }
            else if (rc4Flag)
            {
                ch = rc4_encrypt_byte(&state, ch);
            }
            index++;

            codeStr += huffmanCodes[ch];
        }
        ch = ',';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        else if (xorFlag)
        {
            ch = (ch ^ key[index % keylen]);
        }
        else if (rc4Flag)
        {
            ch = rc4_encrypt_byte(&state, ch);
        }
        index++;
        codeStr += huffmanCodes[ch];
        for (int i = 0; i < strlen(CSNm); i++)
        {
            ch = CSNm[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            else if (xorFlag)
            {
                ch = (ch ^ key[index % keylen]);
            }
            else if (rc4Flag)
            {
                ch = rc4_encrypt_byte(&state, ch);
            }
            index++;
            codeStr += huffmanCodes[ch];
        }
        ch = '\n';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        else if (xorFlag)
        {
            ch = (ch ^ key[index % keylen]);
        }
        else if (rc4Flag)
        {
            ch = rc4_encrypt_byte(&state, ch);
        }
        index++;
        codeStr += huffmanCodes[ch];
        for (int i = 0; i < strlen(CRID); i++)
        {
            ch = CRID[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            else if (xorFlag)
            {
                ch = (ch ^ key[index % keylen]);
            }
            else if (rc4Flag)
            {
                ch = rc4_encrypt_byte(&state, ch);
            }
            index++;
            codeStr += huffmanCodes[ch];
        }
        ch = ',';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        else if (xorFlag)
        {
            ch = (ch ^ key[index % keylen]);
        }
        else if (rc4Flag)
        {
            ch = rc4_encrypt_byte(&state, ch);
        }
        index++;
        codeStr += huffmanCodes[ch];

        for (int i = 0; i < strlen(CRNm); i++)
        {
            ch = CRNm[i];
            if (OffsetFlag)
            {
                ch = (ch + offset) & 0xff;
            }
            else if (xorFlag)
            {
                ch = (ch ^ key[index % keylen]);
            }
            else if (rc4Flag)
            {
                ch = rc4_encrypt_byte(&state, ch);
            }
            index++;
            codeStr += huffmanCodes[ch];
        }
        ch = '\n';
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        else if (xorFlag)
        {
            ch = (ch ^ key[index % keylen]);
        }
        else if (rc4Flag)
        {
            ch = rc4_encrypt_byte(&state, ch);
        }
        index++;
        codeStr += huffmanCodes[ch];
    }

    //���������ļ�
    while (!feof(fp))
    {
        //���ֽڶ�д

        ch = fgetc(fp);
        if (ch == EOF)
            break;
        if (OffsetFlag)
        {
            ch = (ch + offset) & 0xff;
        }
        else if (xorFlag)
        {
            ch = (ch ^ key[index % keylen]);
        }
        else if (rc4Flag)
        {
            ch = rc4_encrypt_byte(&state, ch);
        }
        index++;
        codeStr += huffmanCodes[ch];
        while (codeStr.length() >= 8)
        {
            unsigned char byte = 0;
            for (int i = 0; i < 8; i++)
            {
                byte <<= 1;
                if (codeStr[i] == '1')
                {
                    byte |= 1;
                }
            }
            fwrite(&byte, sizeof(unsigned char), 1, out);
            codeStr.erase(0, 8);
        }
    }
    //д��ʣ���λ��
    if (codeStr.length() > 0)
    {
        unsigned char byte = 0;
        for (int i = 0; i < codeStr.length(); i++)
        {
            byte <<= 1;
            if (codeStr[i] == '1')
            {
                byte |= 1;
            }
        }
        byte <<= 8 - codeStr.length();
        fwrite(&byte, sizeof(unsigned char), 1, out);
    }
    fclose(fp);
    fclose(out);

    return true;
}
bool HuffmanArchiver::AdcInfCheck()
{
    CHuffmanArchiverDlg* pWnd = (CHuffmanArchiverDlg*)AfxGetMainWnd();
    FILE* in = fopen(this->inFileName, "rb");
    if (!in)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::AdcInfCheck():open infile failed");
        return false;
    }
    unsigned char ch;
    std::string adcInfoCode;
    //��ȡ������Ϣ
    if (AdcInfFlag)
    {
        for (int i = 0; i < strlen(CSID); i++)
        {
            ch = CSID[i];
            if (OffsetFlag)
            {
                ch = (offset + ch) & 0xff;
            }
            adcInfoCode += huffmanCodes[ch];
        }
        ch = ',';
        if (OffsetFlag)
        {
            ch = (offset + ch) & 0xff;;
        }
        adcInfoCode += huffmanCodes[ch];
        for (int i = 0; i < strlen(CSNm); i++)
        {
            ch = CSNm[i];
            if (OffsetFlag)
            {
                ch = (offset + ch) & 0xff;
            }
            adcInfoCode += huffmanCodes[ch];
        }
        ch = '\n';
        if (OffsetFlag)
        {
            ch = (offset + ch) & 0xff;
        }
        adcInfoCode += huffmanCodes[ch];
        for (int i = 0; i < strlen(CRID); i++)
        {
            ch = CRID[i];
            if (OffsetFlag)
            {
                ch = (offset + ch) & 0xff;
            }
            adcInfoCode += huffmanCodes[ch];
        }
        ch = ',';
        if (OffsetFlag)
        {
            ch = (offset + ch) & 0xff;
        }
        adcInfoCode += huffmanCodes[ch];
        for (int i = 0; i < strlen(CRNm); i++)
        {
            ch = CRNm[i];
            if (OffsetFlag)
            {
                ch = (offset + ch) & 0xff;
            }
            adcInfoCode += huffmanCodes[ch];
        }
        ch = '\n';
        if (OffsetFlag)
        {
            ch = (offset + ch) & 0xff;
        }
        adcInfoCode += huffmanCodes[ch];
    }

    //std::cout << adcInfoCode << std::endl;
    //�Ա��ļ���ĸ�����Ϣ
    for (int i = 0; i < adcInfoCode.length(); i++)
    {
        int bytePos = i / 8;
        int bitPos = i % 8;
        if (bitPos == 0)
        {
            fread(&ch, sizeof(unsigned char), 1, in);
        }
        if (((ch >> (7 - bitPos)) & 1) != (adcInfoCode[i] - '0'))
        {
            mylog(LogPath, "[Error]:Adc Info Check Error1");
            pWnd->Log(TEXT("[INFO]:������ϢЧ��ʧ��!��ѹ��ֹͣ"));
            return false;
        }
    }
    pWnd->Log(TEXT("[INFO]:������ϢЧ��ɹ�����ʼ��ѹ��"));
    return true;
    
    //char* token;
    //char* Recive = strchr(adcInfo, '\n');
    //Recive += 1;
    //char* last_newline_pos = strrchr(Recive, '\n'); // �������һ�����з���λ��
    //bool res1; bool res2;
    //if (last_newline_pos != nullptr) {
    //    // ����ҵ����һ�����з��������滻Ϊ�ַ��������� '\0'
    //    *last_newline_pos = '\0';
    //}
    //char* tempStr = _strdup(Recive);
    //if (Recive != nullptr)
    //{
    //    token = strtok(tempStr, ",");
    //    if (token != NULL) {
    //       res1=memcmp(token, this->CRID,strlen(this->CRID));
    //    }
    //    token = strtok(NULL, ",");
    //    if (token != NULL)
    //    {
    //        res2 = memcmp(token, this->CRNm, strlen(this->CRID));
    //    }
    //}
    //if (res1 && res2)
    //{
    //    pWnd->Log(TEXT("[INFO]:������ϢЧ��ɹ�����ʼ��ѹ��"));
    //    return true;
    //}
    //else
    //{
    //    pWnd->Log(TEXT("[INFO]:������ϢЧ��ʧ��"));
    //    return false;
    //}
}
bool HuffmanArchiver::readHuffmanCodeTable()
{
    FILE* codeFile = fopen(this->codeFileName, "r");
    if (!codeFile)
    {
        mylog(LogPath, "[Error]:open code file failed");
        return false;
    }

    // ��ȡ�ļ���С
    fscanf(codeFile, "%u", &this->filesize);
    //std::cout<<this->filesize<<std::endl;
    // ��ȡÿ���ַ��ı���
    // ע��ո��λ��
    unsigned int ch;
    unsigned int len;
    while (fscanf(codeFile, "\n0x%02X 0x%02X", &ch, &len) == 2)
    {
        std::string code;
        while (len > 0)
        {
            unsigned int byte;
            fscanf(codeFile, " 0x%02X", &byte);
            for (int i = 7; i >= 0 && len > 0; --i, --len)
            {
                code += ((byte >> i) & 1) ? '1' : '0';
            }
        }
        huffmanCodes[ch] = code;
        huffmanTable[code] = ch;
        //printf("0x%02x",ch); std::cout<<' '+code<<std::endl;
    }

    fclose(codeFile);

    // �ؽ���������
    root = new node();
    for (const auto& pair : huffmanCodes)
    {
        node* currentNode = root;
        for (char bit : pair.second)
        {
            if (bit == '0')
            {
                if (!currentNode->left)
                {
                    currentNode->left = new node();
                }
                currentNode = currentNode->left;
            }
            else
            {
                if (!currentNode->right)
                {
                    currentNode->right = new node();
                }
                currentNode = currentNode->right;
            }
        }
        currentNode->val = pair.first;
    }

    //DEBUG
    // printf("Huffman Tree:\n");

    return true;
}

bool HuffmanArchiver::outDeCompressFile()
{
    //��ѹ���ļ�
    FILE* in = fopen(this->inFileName, "rb");
    if (!in)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::outDeCompressFile:open infile failed");
        return false;
    }

    // ������ļ�
    FILE* out = fopen(this->outFileName, "wb");
    if (!out)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::outDeCompressFile:open outfile failed" );
        fclose(in);
        return false;
    }
    //���ݹ�����������ѹ���ļ�����
    node* currentNode = root;
    unsigned char byte;
    int bitCount = 0;
    bool firstNewline = false;
    bool secondNewline = false;
    int adcInfoIndex = 0;
    bool check = false;
    RC4State rc4State;
    if (rc4Flag)
    {
        rc4_init(&rc4State, this->key, strlen(this->key));
    }
    while (fread(&byte, sizeof(unsigned char), 1, in) == 1)
    {
        if (bitCount >=filesize)
        {
            break;
        }
        for (int i = 7; i >= 0; --i)
        {

            if (byte & (1 << i))
            {
                currentNode = currentNode->right;
            }
            else
            {
                currentNode = currentNode->left;
            }

            if (currentNode->left == nullptr && currentNode->right == nullptr)
            {
                unsigned char val = currentNode->val;
                if (OffsetFlag)
                {
                    val = (val - offset) & 0xff;
                }
                else if (xorFlag)
                {
                    val = (val ^ key[bitCount% this->keylen]);
                }
                else if (rc4Flag)
                {
                    val= rc4_encrypt_byte(&rc4State, val);
                }
                if (AdcInfFlag)
                {
                    if (!firstNewline || !secondNewline)
                    {
                        adcInfo[adcInfoIndex++] = val;
                        if (val == '\n')
                        {
                            if (firstNewline)
                            {
                                secondNewline = true;
                            }
                            else
                            {
                                firstNewline = true;
                            }
                        }
                    }
                    else
                    {
                        fwrite(&val, sizeof(unsigned char), 1, out);
                    }
                }
                else {
                    fwrite(&val, sizeof(unsigned char), 1, out);
                }
                currentNode = root;
                bitCount++;
                if (bitCount >= filesize)
                {
                    break;
                }
            }
        }

    }
    //std::cout << "Bit count: " << bitCount << std::endl;
    //std::cout << "FileSize: " << filesize << std::endl;
    fclose(in);
    fclose(out);
    return true;
}
bool HuffmanArchiver::outDeCompressFile2()
{
    // ��ѹ���ļ�
    FILE* in = fopen(this->inFileName, "rb");
    if (!in)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::outDeCompressFile:open infile failed");
        return false;
    }

    // ������ļ�
    FILE* out = fopen(this->outFileName, "wb");
    if (!out)
    {
        mylog(LogPath, "[Error]:HuffmanArchiver::outDeCompressFile:open outfile failed");
        fclose(in);
        return false;
    }

    // ��ʼ��
    unsigned char byte;
    int bitCount = 0;
    bool firstNewline = false;
    bool secondNewline = false;
    int adcInfoIndex = 0;
    RC4State rc4State;
    if (rc4Flag)
    {
        rc4_init(&rc4State, this->key, strlen(this->key));
    }

    // ���ڴ洢��ǰ��ȡ�ı���
    std::string currentCode;

    while (fread(&byte, sizeof(unsigned char), 1, in) == 1)
    {
        for (int i = 7; i >= 0; --i)
        {
            // ����ǰλ׷�ӵ������ַ�����
            currentCode += (byte & (1 << i)) ? '1' : '0';

            // ��һ�Ƚϵ�ǰ����������������
            for (const auto& entry : huffmanTable)
            {
                if (currentCode == entry.first)
                {
                    unsigned char val = entry.second;
                    if (OffsetFlag)
                    {
                        val = (val - offset) & 0xff;
                    }
                    else if (xorFlag)
                    {
                        val = (val ^ key[bitCount % this->keylen]);
                    }
                    else if (rc4Flag)
                    {
                        val = rc4_encrypt_byte(&rc4State, val);
                    }

                    if (AdcInfFlag)
                    {
                        if (!firstNewline || !secondNewline)
                        {
                            adcInfo[adcInfoIndex++] = val;
                            if (val == '\n')
                            {
                                if (firstNewline)
                                {
                                    secondNewline = true;
                                }
                                else
                                {
                                    firstNewline = true;
                                }
                            }
                        }
                        else
                        {
                            fwrite(&val, sizeof(unsigned char), 1, out);
                        }
                    }
                    else
                    {
                        fwrite(&val, sizeof(unsigned char), 1, out);
                    }

                    // ��յ�ǰ�����ַ���
                    currentCode.clear();
                    bitCount++;
                    if (bitCount >= filesize)
                    {
                        break;
                    }
                    break; // ƥ��ɹ����˳��ڲ�ѭ��
                }
            }
        }
    }

    fclose(in);
    fclose(out);
    return true;
}
bool HuffmanArchiver::compress(bool OffsetFlag, int offset, bool AdcInfFlag, const char* CSID, const char* CSNm, const char* CRID, const char* CRNm,bool xorEncFlag,bool rc4EncFlag,const char* enckey)
{
    // ��ʼ��һ�²���
    this->OffsetFlag = OffsetFlag;
    this->offset = offset;
    this->AdcInfFlag = AdcInfFlag;
    this->CSID = CSID;
    this->CSNm = CSNm;
    this->CRID = CRID;
    this->CRNm = CRNm;
    this->key=enckey;
    this->keylen=strlen(enckey);
    this->xorFlag=xorEncFlag;
    this->rc4Flag=rc4EncFlag;

    countCharFreq();
    buildHuffmanTree();
    buildHuffmanCode();
    outCodeFile();
    outCompressFile();
    return true;
}

bool HuffmanArchiver::decompress(bool OffsetFlag, int offset, bool AdcInfFlag, const char* CSID, const char* CSNm, const char* CRID, const char* CRNm,bool xorDecFlag,bool rc4DecFlag,const char* deckey,bool decompressType)
{
    //readHuffmanCodeTable();
    // ��ȡ�����������
    this->OffsetFlag = OffsetFlag;
    this->offset = offset;
    this->AdcInfFlag = AdcInfFlag;
    this->CSID = CSID;
    this->CSNm = CSNm;
    this->CRID = CRID;
    this->CRNm = CRNm;
    this->key=deckey;
    this->keylen=strlen(deckey);
    this->xorFlag = xorDecFlag;
    this->rc4Flag = rc4DecFlag;
    this->decompressType = decompressType;
    bool res = true;

    if (!readHuffmanCodeTable())
    {
        //����log
        mylog(LogPath, "[Error]:HuffmanArchiver::decompress:readHuffmanCodeTable failed" );
        return false;
    }

    if (AdcInfFlag)
    {
        res = AdcInfCheck();
        if (res)
        {
            mylog(LogPath, "[INFO]:������ϢЧ��ɹ�" );
        }
        else {
            mylog(LogPath, "[Error]:HuffmanArchiver::decompress: AdcInfCheck failed" );
            return false;
        }
    }
    // ��ѹ���ļ�
    if (res)
    {
        //mylog(LogPath, "[Error]:HuffmanArchiver::decompress:outDeCompressFile" );
        if (decompressType)
        {
            outDeCompressFile();
        }
        else {
            outDeCompressFile2();
        }
    }

    return true;
}