// Copy constructor of a layered class.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a35018[];
extern void* g_unk_7101a35248[];

void fn_7100424c08(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35248[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35018[2];
    *reinterpret_cast<u64*>(self + 0x48) = *reinterpret_cast<const u64*>(other + 0x48);
}
