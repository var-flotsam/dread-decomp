// CScorpiusSpikeBallPrickAttack unit (tools/tu_map.py): two identical copy constructors of a layered
// attack class (three vtables stored in turn, each layer's fields copied), shaped like fn_71003e14c0
// (src/game/CBigFistAttack/fn_71003e14c0.cpp). Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a60490[];
extern void* g_unk_7101a60a90[];

static inline __attribute__((always_inline)) void CopyLayers_sbp(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a60490[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 8);
    *reinterpret_cast<void***>(self) = &g_unk_7101a60a90[2];
    __builtin_memcpy(self + 0x30, other + 0x30, 24);
}

void fn_710068bd4c(char* self, const char* other) { CopyLayers_sbp(self, other); }
void fn_710068bda8(char* self, const char* other) { CopyLayers_sbp(self, other); }
