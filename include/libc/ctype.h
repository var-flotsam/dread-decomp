#pragma once

// Minimal <ctype.h> declarations for building library code without a C library.

#ifdef __cplusplus
extern "C" {
#endif

int isalnum(int);
int isalpha(int);
int iscntrl(int);
int isdigit(int);
int islower(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int tolower(int);
int toupper(int);

#ifdef __cplusplus
}
#endif
