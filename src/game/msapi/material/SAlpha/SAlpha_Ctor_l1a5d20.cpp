// msapi::material::SAlpha's unit (tools/tu_map.py unit 229): the material's constructor: the base
// object, two empty ids, two sets of six header blocks, cleared uniform values, empty value block,
// pass and id arrays, and an empty key. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/core/CBaseObject.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/msapi/material/SAlpha.h"
#include "types.h"

using base::global::CRntVector;
using base::global::UnkStringData;

// The material's vtable and its header blocks' vtables.
extern void* g_unk_7101a1e3b8[];
extern void* g_unk_7101a1e3e8[];
extern void* g_unk_7101a1e418[];
extern void* g_unk_7101a1e448[];
extern void* g_unk_7101a1e478[];
extern void* g_unk_7101a1e4a8[];

// A header block over the base object (fn_7100023838 builds it).
template <u32 Size>
struct UnkMaterialBlock_l1a5d20 {
    void** vtable;
    u8 pad_0x8[Size - 8];

    void Construct(void** vtable_base) {
        fn_7100023838(this);
        vtable = vtable_base + 2;
    }
};

// One set of the six header blocks.
struct UnkMaterialBlocks_l1a5d20 {
    UnkMaterialBlock_l1a5d20<0x18> block_a;
    UnkMaterialBlock_l1a5d20<0x10> block_b;
    UnkMaterialBlock_l1a5d20<0x28> block_c;
    UnkMaterialBlock_l1a5d20<0x18> block_d;
    UnkMaterialBlock_l1a5d20<0x10> block_e;
    UnkMaterialBlock_l1a5d20<0x18> block_f;

    void Construct() {
        block_a.Construct(g_unk_7101a1e3b8);
        block_b.Construct(g_unk_7101a1e3e8);
        block_c.Construct(g_unk_7101a1e418);
        block_d.Construct(g_unk_7101a1e448);
        block_e.Construct(g_unk_7101a1e478);
        block_f.Construct(g_unk_7101a1e4a8);
    }
};
static_assert(sizeof(UnkMaterialBlocks_l1a5d20) == 0x90, "layout");

// A named value block (0x38 bytes) and a pass (0x60 bytes); only their size matters here.
struct UnkMaterialValue_l1a5d20 {
    u8 pad_0x0[0x38];
};
struct UnkMaterialPass_l1a5d20 {
    u8 pad_0x0[0x60];
};

struct UnkMaterial_l1a5d20 {
    void** vtable;
    UnkCountedResource* resource;  // 0x8
    UnkStringData* field_0x10;
    u8 pad_0x18[0x8];
    UnkStringData* field_0x20;
    UnkMaterialBlocks_l1a5d20 blocks;  // 0x28
    UnkMaterialBlocks_l1a5d20 copies;  // 0xb8
    u32 field_0x148;
    f32 stage1_values[10][4];  // 0x14c
    u8 pad_0x1ec[0x54];
    f32 stage0_values[10][4];  // 0x240
    u8 pad_0x2e0[0x50];
    CRntVector<UnkMaterialValue_l1a5d20> values;  // 0x330
    CRntVector<UnkMaterialPass_l1a5d20> passes;  // 0x358
    u8 pad_0x380[0x8];
    UnkCountedResource* field_0x388;
    void* handle;  // 0x390
    UnkStringData* key;  // 0x398
    u32 field_0x3a0;
    u16 field_0x3a4;
    CRntVector<base::global::CStrId> ids;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkMaterial_l1a5d20, values) == 0x330, "layout");
static_assert(__builtin_offsetof(UnkMaterial_l1a5d20, ids) == 0x3a8, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71001a5d20(UnkMaterial_l1a5d20* self) {
    fn_7100023838(self);
    self->vtable = g_unk_7101a1e388 + 2;
    self->resource = nullptr;
    MakeEmpty(&self->field_0x10);
    MakeEmpty(&self->field_0x20);
    self->blocks.Construct();
    self->copies.Construct();
    for (u32 i = 0; i < 10; ++i)
        for (u32 j = 0; j < 4; ++j)
            self->stage1_values[i][j] = 0.0f;
    for (u32 i = 0; i < 10; ++i)
        for (u32 j = 0; j < 4; ++j)
            self->stage0_values[i][j] = 0.0f;
    new (&self->values) CRntVector<UnkMaterialValue_l1a5d20>(0x35);
    new (&self->passes) CRntVector<UnkMaterialPass_l1a5d20>(0x35);
    self->field_0x388 = nullptr;
    self->handle = nullptr;
    MakeEmpty(&self->key);
    self->field_0x3a0 = 0;
    self->field_0x3a4 = 0;
    new (&self->ids) CRntVector<base::global::CStrId>(0xf);
}
