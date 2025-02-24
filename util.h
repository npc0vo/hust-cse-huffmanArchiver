#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <cstdio>
#define LogPath "log.txt"
// RC4 ×´Ì¬½á¹¹Ìå
typedef struct {
    unsigned char S[256];
    int i;
    int j;
} RC4State;
void rc4_init(RC4State* state, const char* key, int key_length);
unsigned char rc4_encrypt_byte(RC4State* state, unsigned char byte);
int mylog(const char* filename, const char* format, ...);
uint64_t fnv1a_64(const char* filename);
int readLast16Bytes(const char* filename,unsigned char* buffer);