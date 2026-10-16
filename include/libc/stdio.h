#pragma once

// Minimal <stdio.h> declarations for building library code without a C library.
// BUFSIZ sets Lua's LUAL_BUFFERSIZE (luaL_Buffer layout). 1024 is a guess (musl's value);
// confirm it against string-library stack frames before relying on it.

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UnkFile FILE;

#define BUFSIZ 1024
#define EOF (-1)

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

int printf(const char*, ...);
int fprintf(FILE*, const char*, ...);
int sprintf(char*, const char*, ...);
int snprintf(char*, size_t, const char*, ...);
int vsnprintf(char*, size_t, const char*, va_list);
int fputs(const char*, FILE*);
int fflush(FILE*);
char* fgets(char*, int, FILE*);
FILE* fopen(const char*, const char*);
FILE* freopen(const char*, const char*, FILE*);
int fclose(FILE*);
size_t fread(void*, size_t, size_t, FILE*);
int getc(FILE*);
int ungetc(int, FILE*);
int ferror(FILE*);
int feof(FILE*);

#ifdef __cplusplus
}
#endif
