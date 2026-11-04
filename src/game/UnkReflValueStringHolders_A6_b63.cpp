// Destructors (complete and deleting) of another class of unit 1734 (tools/tu_map.py) over the base of
// src/game/UnkReflValueStringHolders.cpp, with its own vtable (g_unk_7101aa01f0): free the string with inline
// storage at +0xf0 (fn_710007ed40, then fn_710007ecd4) and the string at +0xa8, then the base part restores its
// vtable (g_unk_7101a9cd68) and destroys its reflected value at +0x18 (fn_71000a1c60). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101a9cd68[];  // the base part's vtable
extern char g_unk_7101aa01f0[];

struct UnkValueStringHolder_A6_b63 {
    void** vtable;
    u8 unk_0x8[0x10];
    base::reflection::UnkReflValue value;  // 0x18
    u8 unk_0x18_end[0xa8 - 0x18 - sizeof(base::reflection::UnkReflValue)];
    CRntString str;  // 0xa8
    u8 unk_0xa8_end[0xf0 - 0xa8 - sizeof(CRntString)];
    CRntString inline_str;  // 0xf0
};

static inline __attribute__((always_inline)) void Destroy_A6_b63(UnkValueStringHolder_A6_b63*& self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa01f0) + 2;
    fn_710007ed40(&self->inline_str);
    fn_710007ecd4(&self->inline_str);
    asm("" : "+r"(self));  // the original adds the +0xa8 offset after the calls
    fn_710007ecd4(&self->str);
    self->vtable = g_unk_7101a9cd68 + 2;
    fn_71000a1c60(&self->value);
}

void fn_7100b94fe8(UnkValueStringHolder_A6_b63* self) {
    Destroy_A6_b63(self);
}

void fn_7100b95044(UnkValueStringHolder_A6_b63* self) {
    Destroy_A6_b63(self);
    operator delete(self);
}
