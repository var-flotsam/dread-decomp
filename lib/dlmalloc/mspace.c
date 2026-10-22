/* dlmalloc 2.8.4 (malloc.c, unmodified) as the game builds it: see README.md. */
#define ONLY_MSPACES 1
#define USE_LOCKS 0
#define HAVE_MMAP 0
#define HAVE_MORECORE 0
#define MORECORE_CANNOT_TRIM 1
#define MALLOC_ALIGNMENT 16
#define MALLOC_FAILURE_ACTION
#define CORRUPTION_ERROR_ACTION(m)
#define USAGE_ERROR_ACTION(m, p)
#define malloc_getpagesize ((size_t)4096U)
#include <time.h>
/* The game's mparams.magic is the constant 0x58585858, which init_mparams computes from this. */
#define time(t) ((time_t)0x0D0D0D0D)
#include "malloc.c"
