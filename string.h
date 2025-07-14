#ifndef STRING_H
#define STRING_H

int strcmp(const char* s1, const char* s2);
int strlen(const char* str);
void* memset(void* dest, int val, unsigned int len);
void* memcpy(void* dest, const void* src, unsigned int len);

#endif
