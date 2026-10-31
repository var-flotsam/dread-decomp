// Destructors (complete and deleting) of two of the classes in unit 1734 of tools/tu_map.py over one
// base: each frees a string with inline storage (fn_710007ed40, then fn_710007ecd4) and a string
// at its own offsets, then the base part restores its vtable (g_unk_7101a9cd68) and destroys its
// reflected value at +0x18 (fn_71000a1c60). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101a9cd68[];  // the base part's vtable

struct UnkValueHolderBase {
    void** vtable;
    u8 unk_0x8[0x10];
    base::reflection::UnkReflValue value;

    ~UnkValueHolderBase() {
        vtable = g_unk_7101a9cd68 + 2;
        fn_71000a1c60(&value);
    }
};

template <char* Vtable, u32 StrOffset, u32 InlineStrOffset>
struct UnkValueStringHolder : UnkValueHolderBase {
    CRntString* At(u32 offset) { return reinterpret_cast<CRntString*>(reinterpret_cast<char*>(this) + offset); }

    ~UnkValueStringHolder() {
        vtable = reinterpret_cast<void**>(Vtable) + 2;
        CRntString* inline_str = At(InlineStrOffset);
        fn_710007ed40(inline_str);
        fn_710007ecd4(inline_str);
        fn_710007ecd4(At(StrOffset));
    }
};

extern char g_unk_7101aa0190[];
extern char g_unk_7101aa0250[];
using UnkValueStringHolderB0 = UnkValueStringHolder<g_unk_7101aa0190, 0xb0, 0xf8>;
using UnkValueStringHolderB8 = UnkValueStringHolder<g_unk_7101aa0250, 0xb8, 0x100>;

void fn_7100b94f28(UnkValueStringHolderB0* self) { self->~UnkValueStringHolderB0(); }
void fn_7100b94f84(UnkValueStringHolderB0* self) {
    self->~UnkValueStringHolderB0();
    operator delete(self);
}
void fn_7100b950a8(UnkValueStringHolderB8* self) { self->~UnkValueStringHolderB8(); }
void fn_7100b95110(UnkValueStringHolderB8* self) {
    self->~UnkValueStringHolderB8();
    operator delete(self);
}
