#include <pch.h>
#include <util.h>	
// FNV-1a 64λ��ϣ�㷨�ĳ�ʼֵ
#define FNV1A_64_INIT 0xcbf29ce484222325ULL
// FNV-1a 64λ��ϣ�㷨������
#define FNV1A_64_PRIME 0x100000001b3
//��ȡָ���ļ���hashֵ
/**
 * ����ʽ�����ַ���д�뵽��־�ļ��С�
 *
 * @param filename ��־�ļ���·��������ʹ��Windows����·���ָ�����
 * @param format ��ʽ���ַ�����
 * @param ... �ɱ�����б�
 * @return �ɹ����طǸ�ֵ��ʧ�ܷ��ظ�ֵ��
 */
int mylog(const char* filename, const char* format, ...) {
    FILE* file;
    va_list args;
    int result;

    // ���ļ�������ļ��������򴴽�������ļ��Ѵ�����׷�����ݡ�
    file = fopen(filename, "a"); // "a" ģʽ��ʾ׷��ģʽ��
    if (file == NULL) {
        perror("Error opening file");
        return -1; // �ļ���ʧ�ܡ�
    }

    // ��ʼ���ɱ�����б�
    va_start(args, format);

    // ʹ��vfprintf����ʽ���ַ���д���ļ���
    result = vfprintf(file, format, args);

    // ����ɱ�����б�
    va_end(args);

    // �ر��ļ���
    fclose(file);

    return result;
}

/**
 * ����ȡָ���ļ���fnv1a-64hash��
 *
 * @param filename �ļ���·��������ʹ��Windows����·���ָ�����
 * @return �ɹ����طǸ�ֵ��ʧ�ܷ��ظ�ֵ��
 */
uint64_t fnv1a_64(const char* filename) {
    uint64_t hash = FNV1A_64_INIT;
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }
    int byte;
    // ���ֽڶ�ȡ�ļ������¹�ϣֵ
    while ((byte = fgetc(fp)) != EOF) {
        hash ^= (uint8_t)byte; // ���ֽ�ת��Ϊ uint8_t �����
        hash *= FNV1A_64_PRIME; // ��������
    }

    fclose(fp);
    return hash;
}


/**
 * ��ȡָ���ļ������16���ֽ�
 * @param filename �ļ���
 * @param buffer �洢��ȡ����Ļ�����
 * @return �ɹ����ض�ȡ���ֽ�����ʧ�ܷ���-1
 */
int readLast16Bytes(const char* filename,unsigned char* buffer) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1; // ���ļ�ʧ��
    }

    // ��ȡ�ļ���С
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    // �����ļ���С������ȡ����
    int bytesRead = 0;
    if (fileSize < 16) {
        // �ļ�С��16�ֽڣ���ͷ��ʼ��ȡ
        fseek(file, 0, SEEK_SET);
        bytesRead = fread(buffer, sizeof(char), fileSize, file);
    }
    else {
        // �ļ����ڵ���16�ֽڣ���ȡ���16���ֽ�
        fseek(file, -16, SEEK_END);
        bytesRead = fread(buffer, sizeof(char), 16, file);
    }

    fclose(file); 
    return bytesRead; 
}




// ��ʼ�� RC4 ״̬
void rc4_init(RC4State* state, const char* key, int key_length) {
    for (int i = 0; i < 256; i++) {
        state->S[i] = i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + state->S[i] + key[i % key_length]) % 256;
        // ���� S[i] �� S[j]
        unsigned char temp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = temp;
    }

    state->i = 0;
    state->j = 0;
}

// ������Կ���ֽڲ�����/����
unsigned char rc4_encrypt_byte(RC4State* state, unsigned char byte) {
    state->i = (state->i + 1) % 256;
    state->j = (state->j + state->S[state->i]) % 256;
    // ���� S[i] �� S[j]
    unsigned char temp = state->S[state->i];
    state->S[state->i] = state->S[state->j];
    state->S[state->j] = temp;

    // ������Կ���ֽ�
    unsigned char key_byte = state->S[(state->S[state->i] + state->S[state->j]) % 256];

    // ����/�����ֽ�
    return (byte ^ key_byte)&0xff;
}