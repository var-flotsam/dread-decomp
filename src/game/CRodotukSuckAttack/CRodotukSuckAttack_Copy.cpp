// CRodotukSuckAttack: two identical copies of a layered value's copy constructor (fn_710066c884 and
// fn_710066c8f8, the reflection descriptor's slots 0x30 and 0x38): the base part (vtable
// g_unk_7101a24060, 0x20 bytes at +0x8), then its own vtable g_unk_7101a5da10, the member at +0x28
// (fn_710066c9d8), 0x12 bytes at +0x58 and the member at +0x70
// (fn_7100d62b68). Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a5da10[];

void fn_710066c9d8(char* self, const char* other);
void fn_7100d62b68(char* self, const char* other);

namespace {
__attribute__((always_inline)) inline void CopyConstruct(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a5da10[2];
    fn_710066c9d8(self + 0x28, other + 0x28);
    __builtin_memcpy(self + 0x58, other + 0x58, 18);
    fn_7100d62b68(self + 0x70, other + 0x70);
}
}  // namespace

void fn_710066c884(void* dst, const void* src) {
    CopyConstruct(static_cast<char*>(dst), static_cast<const char*>(src));
}

void fn_710066c8f8(void* dst, const void* src) {
    CopyConstruct(static_cast<char*>(dst), static_cast<const char*>(src));
}
