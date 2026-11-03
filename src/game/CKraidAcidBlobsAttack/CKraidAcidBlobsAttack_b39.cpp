// CKraidAcidBlobsAttack unit (tools/tu_map.py): copy constructors of layered attack classes (three
// vtables stored in turn, each layer's fields copied), shaped like fn_71003e14c0
// (src/game/CBigFistAttack/fn_71003e14c0.cpp). Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a53b28[];
extern void* g_unk_7101a53a30[];

static inline __attribute__((always_inline)) void CopyLayers_kab(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a53b28[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 8);
    *reinterpret_cast<void***>(self) = &g_unk_7101a53a30[2];
    __builtin_memcpy(self + 0x30, other + 0x30, 28);
}

// Two identical copies of the same copy constructor.
void fn_71005f1ab8(char* self, const char* other) { CopyLayers_kab(self, other); }
void fn_71005f1b1c(char* self, const char* other) { CopyLayers_kab(self, other); }
