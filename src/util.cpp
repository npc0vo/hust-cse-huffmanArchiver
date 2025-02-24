#include <pch.h>
#include <util.h>	
// FNV-1a 64位哈希算法的初始值
#define FNV1A_64_INIT 0xcbf29ce484222325ULL
// FNV-1a 64位哈希算法的质数
#define FNV1A_64_PRIME 0x100000001b3
//获取指定文件的hash值
/**
 * 将格式化的字符串写入到日志文件中。
 *
 * @param filename 日志文件的路径，可以使用Windows风格的路径分隔符。
 * @param format 格式化字符串。
 * @param ... 可变参数列表。
 * @return 成功返回非负值，失败返回负值。
 */
int mylog(const char* filename, const char* format, ...) {
    FILE* file;
    va_list args;
    int result;

    // 打开文件，如果文件不存在则创建，如果文件已存在则追加内容。
    file = fopen(filename, "a"); // "a" 模式表示追加模式。
    if (file == NULL) {
        perror("Error opening file");
        return -1; // 文件打开失败。
    }

    // 初始化可变参数列表。
    va_start(args, format);

    // 使用vfprintf将格式化字符串写入文件。
    result = vfprintf(file, format, args);

    // 清理可变参数列表。
    va_end(args);

    // 关闭文件。
    fclose(file);

    return result;
}

/**
 * 将获取指定文件的fnv1a-64hash。
 *
 * @param filename 文件的路径，可以使用Windows风格的路径分隔符。
 * @return 成功返回非负值，失败返回负值。
 */
uint64_t fnv1a_64(const char* filename) {
    uint64_t hash = FNV1A_64_INIT;
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0;
    }
    int byte;
    // 逐字节读取文件并更新哈希值
    while ((byte = fgetc(fp)) != EOF) {
        hash ^= (uint8_t)byte; // 将字节转换为 uint8_t 并异或
        hash *= FNV1A_64_PRIME; // 乘以质数
    }

    fclose(fp);
    return hash;
}


/**
 * 读取指定文件的最后16个字节
 * @param filename 文件名
 * @param buffer 存储读取结果的缓冲区
 * @return 成功返回读取的字节数，失败返回-1
 */
int readLast16Bytes(const char* filename,unsigned char* buffer) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1; // 打开文件失败
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    // 根据文件大小决定读取策略
    int bytesRead = 0;
    if (fileSize < 16) {
        // 文件小于16字节，从头开始读取
        fseek(file, 0, SEEK_SET);
        bytesRead = fread(buffer, sizeof(char), fileSize, file);
    }
    else {
        // 文件大于等于16字节，读取最后16个字节
        fseek(file, -16, SEEK_END);
        bytesRead = fread(buffer, sizeof(char), 16, file);
    }

    fclose(file); 
    return bytesRead; 
}




// 初始化 RC4 状态
void rc4_init(RC4State* state, const char* key, int key_length) {
    for (int i = 0; i < 256; i++) {
        state->S[i] = i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + state->S[i] + key[i % key_length]) % 256;
        // 交换 S[i] 和 S[j]
        unsigned char temp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = temp;
    }

    state->i = 0;
    state->j = 0;
}

// 生成密钥流字节并加密/解密
unsigned char rc4_encrypt_byte(RC4State* state, unsigned char byte) {
    state->i = (state->i + 1) % 256;
    state->j = (state->j + state->S[state->i]) % 256;
    // 交换 S[i] 和 S[j]
    unsigned char temp = state->S[state->i];
    state->S[state->i] = state->S[state->j];
    state->S[state->j] = temp;

    // 生成密钥流字节
    unsigned char key_byte = state->S[(state->S[state->i] + state->S[state->j]) % 256];

    // 加密/解密字节
    return (byte ^ key_byte)&0xff;
}