// Copy constructor of a layered class.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a2c478[];

void fn_71003be300(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a2c478[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 24);
}
