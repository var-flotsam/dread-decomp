// Copy constructor of a layered class.
#include "game/UnkRefObj.h"
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a3c6c8[];
extern void* g_unk_7101a3c8b8[];

void fn_710049500c(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3c6c8[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 8);
    __builtin_memcpy(self + 0x30, other + 0x30, 4);
    __builtin_memcpy(self + 0x34, other + 0x34, 4);
    __builtin_memcpy(self + 0x38, other + 0x38, 4);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3c8b8[2];
    __builtin_memcpy(self + 0x40, other + 0x40, 8);
}
