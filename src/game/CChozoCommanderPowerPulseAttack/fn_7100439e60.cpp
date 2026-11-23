// Copy constructor of a layered class.
#include "game/CChozoCommanderPowerPulseAttack.h"
#include "game/UnkRefObj.h"
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a35248[];
extern void* g_unk_7101a356c8[];

void fn_7100439e60(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35248[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a356c8[2];
    __builtin_memcpy(self + 0x48, other + 0x48, 8);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35f60[2];
    __builtin_memcpy(self + 0x50, other + 0x50, 38);
}
