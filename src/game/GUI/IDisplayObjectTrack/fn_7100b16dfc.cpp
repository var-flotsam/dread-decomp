// Constructors of three classes over a common base (vtable, then an empty name string): the
// members are zeroed first, then initialised (a pair of values 1 and -1 and an array with flags
// 0xf), then the class's own vtable is set.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

// The object, value-initialised (zeroed, then the base's vtable and the initialisers run).
template <void** Base>
struct UnkNamedListFields {
    void** vtable = Base + 2;
    CRntString name = CRntString("");
    u32 field_0x38 = 1;
    s32 field_0x3c = -1;
    base::UnkVector<void*> list{0xf};
};
static_assert(sizeof(UnkNamedListFields<nullptr>) == 0x68, "UnkNamedListFields layout");

struct UnkNamedList {
    void** vtable;
    u8 unk_0x8[0x60];
};

template <void** Base, void** Vtable>
__attribute__((always_inline)) static inline void Construct(UnkNamedList* self) {
    auto* fields = new (self) UnkNamedListFields<Base>();
    fields->vtable = Vtable + 2;
}

extern void* g_unk_7101a9a508[];
extern void* g_unk_7101a9a4c0[];
extern void* g_unk_7101a9a598[];
extern void* g_unk_7101a9a550[];
extern void* g_unk_7101a9a628[];
extern void* g_unk_7101a9a5e0[];

void fn_7100b16dfc(UnkNamedList* self) { Construct<g_unk_7101a9a508, g_unk_7101a9a4c0>(self); }
void fn_7100b1854c(UnkNamedList* self) { Construct<g_unk_7101a9a598, g_unk_7101a9a550>(self); }
void fn_7100b19d34(UnkNamedList* self) { Construct<g_unk_7101a9a628, g_unk_7101a9a5e0>(self); }
