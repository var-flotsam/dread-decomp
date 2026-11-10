// CPowerUpLifeComponent's unit (tools/tu_map.py): the constructor (fn_71008e4b24). The base constructor
// fn_71008e04e8, the vtable, the callback list at +0x388 (an empty array of callables with step 10 and flags
// 0xf from the default allocator, under the list's two vtables, as fn_7100892528 builds its entries), a
// cleared handle at +0x3c8, a string id built from "" (fn_71000003d4) at +0x3d0 and an empty string id at
// +0x3d8. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

extern void* g_unk_7101a20998[];  // the list base's vtable
extern void* g_unk_7101a33fa0[];  // the list's vtable
extern char g_unk_7101a7f4c0[];   // the component's vtable

struct UnkLifeComponent_A5_b71;
void fn_71008e04e8(UnkLifeComponent_A5_b71* self);

struct UnkPowerUpLifeListBase_ovb0126 {
    char* vtable;
    UnkOwnedErasedVector callables;
    u32 field_0x30;

    UnkPowerUpLifeListBase_ovb0126() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), field_0x30(0) {}
};

struct UnkPowerUpLifeList_ovb0126 : UnkPowerUpLifeListBase_ovb0126 {
    void* owner;

    UnkPowerUpLifeList_ovb0126() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner = nullptr;
    }
};

struct UnkPowerUpLifeEmptyId_ovb0126 {
    UnkStringData* data;

    UnkPowerUpLifeEmptyId_ovb0126() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

struct UnkPowerUpLife_ovb0126 {
    char* vtable;
    u8 pad_0x8[0x380];
    UnkPowerUpLifeList_ovb0126 list;  // 0x388
    void* handle;                     // 0x3c8
    base::global::CStrId name;        // 0x3d0
    UnkPowerUpLifeEmptyId_ovb0126 id;  // 0x3d8
};
static_assert(__builtin_offsetof(UnkPowerUpLife_ovb0126, id) == 0x3d8, "layout");

void fn_71008e4b24(UnkPowerUpLife_ovb0126* self) {
    fn_71008e04e8(reinterpret_cast<UnkLifeComponent_A5_b71*>(self));
    self->vtable = g_unk_7101a7f4c0 + 0x10;
    new (&self->list) UnkPowerUpLifeList_ovb0126;
    self->handle = nullptr;
    new (&self->name) base::global::CStrId(g_unk_710162e2ed, true);
    new (&self->id) UnkPowerUpLifeEmptyId_ovb0126;
}
