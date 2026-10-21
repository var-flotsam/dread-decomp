// Sets a global flag; when it changes, also sets two other global flags and calls a global
// function pointer for each of the eight slots 0-7 of a global object with the new value.
#include "types.h"

// All reached through the GOT (named after their GOT slots).
extern bool g_unk_7101cc5e60;
extern bool g_unk_7101cc5da8;
extern bool g_unk_7101cc5dc0;
extern void (*g_unk_7101cc5520)(void* object, s32 slot, bool value);
extern char g_unk_7101cc5ce8[];

void fn_710015a3e0(bool value) {
    if (g_unk_7101cc5e60 == value)
        return;
    g_unk_7101cc5da8 = true;
    g_unk_7101cc5dc0 = true;
    g_unk_7101cc5e60 = value;
    for (s32 i = 0; i < 8; ++i)
        g_unk_7101cc5520(g_unk_7101cc5ce8, i, value);
}
