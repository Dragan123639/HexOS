#include "string.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void* memset(void* dest, int val, unsigned int len) {
    unsigned char* ptr = (unsigned char*)dest;
    while (len-- > 0)
        *ptr++ = (unsigned char)val;
    return dest;
}

void* memcpy(void* dest, const void* src, unsigned int len) {
    const unsigned char* sp = src;
    unsigned char* dp = dest;
    while (len--) *dp++ = *sp++;
    return dest;
}
