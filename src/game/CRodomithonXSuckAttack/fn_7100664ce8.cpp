// Copies of fn_71003be300 (src/game/CAutomperAutomaticIrradiationAttack/fn_71003be300.cpp) in the
// CRodomithonXSuckAttack unit: the copy constructor of a layered class whose derived part has 20
// bytes (fn_7100664ce8 and an identical fn_7100664d2c), with this unit's vtable for that part.
#include "types.h"

extern void* g_unk_7101a24060[];  // the base part's vtable
extern void* g_unk_7101a5cfc0[];  // the derived part's vtable

static inline void CopyConstruct(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a5cfc0[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 20);
}

void fn_7100664ce8(char* self, const char* other) { CopyConstruct(self, other); }

void fn_7100664d2c(char* self, const char* other) { CopyConstruct(self, other); }
