// CMaterialFXComponent unit (tools/tu_map.py): the constructor: the base constructor (fn_71009f1910), the
// vtable (g_unk_7101a833b0), an empty list, then 50 default effect records (each holding a copy of the
// empty id) and the list's pointers to them. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CSceneComponent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a833b0[];

struct UnkBreakableTileGroup_b46a2;

struct UnkMaterialFXEntry_l92623c {
    u32 field_0x0 = 0;
    u32 slot = 0x7fffffff;  // 0x4
    u32 kind;               // 0x8
    f32 field_0xc[4] = {};
    f32 field_0x1c[4] = {};
    u8 pad_0x2c[0x4];
    u64 field_0x30 = 0;
    CStrId id = g_unk_7101d470e0;  // 0x38
    u32 index = 0xffffffff;        // 0x40
    u32 type = 3;                  // 0x44
    u32 field_0x48 = 0;
    f32 field_0x4c = 0.0f;
    u32 field_0x50 = 0;
};
static_assert(sizeof(UnkMaterialFXEntry_l92623c) == 0x58, "UnkMaterialFXEntry_l92623c size");

struct UnkMaterialFXComponent_l92623c {
    void** vtable;
    u8 pad_0x8[0x40];
    u32 count;  // 0x48
    u8 pad_0x4c[0x4];
    UnkMaterialFXEntry_l92623c* entries[50];  // 0x50
    UnkMaterialFXEntry_l92623c storage[50];   // 0x1e0
};

void fn_710092623c(UnkMaterialFXComponent_l92623c* self) {
    fn_71009f1910(reinterpret_cast<UnkBreakableTileGroup_b46a2*>(self));
    self->vtable = reinterpret_cast<void**>(g_unk_7101a833b0 + 0x10);
    self->count = 0;
    new (self->storage) UnkMaterialFXEntry_l92623c[50];
    for (u32 i = 0; i < 50; ++i)
        self->entries[i] = &self->storage[i];
}
