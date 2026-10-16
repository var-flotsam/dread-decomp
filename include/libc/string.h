#pragma once

// Minimal <string.h> declarations for building library code without a C library.

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char*);
int strcmp(const char*, const char*);
int strncmp(const char*, const char*, size_t);
int strcoll(const char*, const char*);
char* strchr(const char*, int);
char* strrchr(const char*, int);
char* strstr(const char*, const char*);
char* strpbrk(const char*, const char*);
size_t strspn(const char*, const char*);
char* strcpy(char*, const char*);
char* strncpy(char*, const char*, size_t);
char* strcat(char*, const char*);
char* strerror(int);
void* memcpy(void*, const void*, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
int memcmp(const void*, const void*, size_t);
void* memchr(const void*, int, size_t);

#ifdef __cplusplus
}
#endif
