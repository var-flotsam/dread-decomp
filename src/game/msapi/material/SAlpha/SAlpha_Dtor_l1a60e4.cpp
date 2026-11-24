// msapi::material::SAlpha's unit (tools/tu_map.py unit 229): the material's destructor: empties it
// (fn_71001a6418), frees its ids, value blocks and passes, releases its key and resources, destroys
// its header blocks and runs the base destructor. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/core/CBaseObject.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/msapi/material/SAlpha.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkMaterialReset_ovb0098;

// An element destroyed through its vtable's slot 1 (the complete destructor).
struct UnkMaterialElem_l1a60e4 {
    virtual void vf_0x0();
    virtual ~UnkMaterialElem_l1a60e4();
};
// A named value block (0x38 bytes) and a pass (0x60 bytes).
struct UnkMaterialValue_l1a60e4 : UnkMaterialElem_l1a60e4 {
    u8 pad_0x8[0x30];
};
struct UnkMaterialPass_l1a60e4 : UnkMaterialElem_l1a60e4 {
    u8 pad_0x8[0x58];
};

struct UnkMaterial_l1a60e4 {
    void** vtable;
    UnkCountedResource* resource;  // 0x8
    CStrId field_0x10;
    u8 pad_0x18[0x8];
    CStrId field_0x20;
    // Header blocks, each over the base object (fn_710002384c destroys them).
    u8 block_0x28[0x18];
    u8 block_0x40[0x10];
    u8 block_0x50[0x28];
    u8 block_0x78[0x18];
    u8 block_0x90[0x10];
    u8 block_0xa0[0x18];
    u8 block_0xb8[0x18];
    u8 block_0xd0[0x10];
    u8 block_0xe0[0x28];
    u8 block_0x108[0x18];
    u8 block_0x120[0x10];
    u8 block_0x130[0x200];
    CRntVector<UnkMaterialValue_l1a60e4> values;  // 0x330
    CRntVector<UnkMaterialPass_l1a60e4> passes;  // 0x358
    u8 pad_0x380[0x8];
    UnkCountedResource* field_0x388;
    u8 pad_0x390[0x8];
    CStrId key;  // 0x398
    u8 pad_0x3a0[0x8];
    CRntVector<CStrId> ids;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkMaterial_l1a60e4, values) == 0x330, "layout");
static_assert(__builtin_offsetof(UnkMaterial_l1a60e4, ids) == 0x3a8, "layout");

static inline void Release(UnkCountedResource* resource) {
    if (resource && --resource->ref_count == 0)
        fn_710001737c(resource);
}

void fn_71001a60e4(UnkMaterial_l1a60e4* self) {
    self->vtable = g_unk_7101a1e388 + 2;
    SAlpha_UnregisterAndClearMaterial(reinterpret_cast<UnkMaterialReset_ovb0098*>(self));
    if (self->ids.data) {
        for (u64 i = 0; i < self->ids.count; ++i)
            self->ids.data[i].Reset();
        self->ids.count = 0;
        self->ids.allocator->vf_0x48(self->ids.data);
        self->ids.capacity = 0;
    }
    self->key.Reset();
    Release(self->field_0x388);
    self->passes.Free();
    self->values.Free();
    fn_710002384c(self->block_0x130);
    fn_710002384c(self->block_0x120);
    fn_710002384c(self->block_0x108);
    fn_710002384c(self->block_0xe0);
    fn_710002384c(self->block_0xd0);
    fn_710002384c(self->block_0xb8);
    fn_710002384c(self->block_0xa0);
    fn_710002384c(self->block_0x90);
    fn_710002384c(self->block_0x78);
    fn_710002384c(self->block_0x50);
    fn_710002384c(self->block_0x40);
    fn_710002384c(self->block_0x28);
    self->field_0x20.Reset();
    self->field_0x10.Reset();
    Release(self->resource);
    fn_710002384c(self);
}
