#pragma once

// Minimal <stdlib.h> declarations for building library code without a C library.

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RAND_MAX 0x7fffffff
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int rand(void);
void srand(unsigned int);
void* malloc(size_t);
void* realloc(void*, size_t);
void free(void*);
void abort(void);
void exit(int);
char* getenv(const char*);
double strtod(const char*, char**);
float strtof(const char*, char**);
long strtol(const char*, char**, int);
unsigned long strtoul(const char*, char**, int);
int abs(int);

#ifdef __cplusplus
}
#endif
