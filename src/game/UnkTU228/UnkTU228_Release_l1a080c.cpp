// Unit 228: releases a decal resource back to its manager: frees the manager's slots its texture
// bindings hold, drops the in-use count, empties and destroys it and returns it to the pool.
// Names and layouts are placeholders.
#include "game/UnkTU228.h"
#include "game/msapi/material/SAlpha.h"
#include "game/operator_new.h"
#include "types.h"

struct UnkMaterialReset_ovb0098;

struct UnkDecalStat_l1a080c {
    u64 field_0x0;
    u64 released;
};
struct UnkDecalStats_l1a080c;
UnkDecalStat_l1a080c& fn_71001a1020(UnkDecalStats_l1a080c* self, u32 index);

// One binding of a resource: type 'f' with a level 1 to 4 holds a slot keyed by `key`.
struct UnkDecalBinding_l1a080c {
    u8 pad_0x0[0x10];
    u8 type;  // +0x10
    u8 pad_0x11[3];
    u32 level;  // +0x14
    u8 pad_0x18[0x18];
    const void* key;  // +0x30
};
static_assert(sizeof(UnkDecalBinding_l1a080c) == 0x38, "");

struct UnkDecalBindingList_l1a080c {
    UnkDecalBinding_l1a080c* data;
    u32 count;
    u32 pad_0xc;

    UnkDecalBinding_l1a080c* begin() { return data; }
    UnkDecalBinding_l1a080c* end() { return data + count; }
};

struct UnkDecalPass_l1a080c {
    u8 pad_0x0[0x10];
    UnkDecalBindingList_l1a080c bindings;  // +0x10
    u8 pad_0x20[0x40];
};
static_assert(sizeof(UnkDecalPass_l1a080c) == 0x60, "");

struct UnkDecalPassList_l1a080c {
    UnkDecalPass_l1a080c* data;
    u32 count;
    u32 pad_0xc;

    UnkDecalPass_l1a080c* begin() { return data; }
    UnkDecalPass_l1a080c* end() { return data + count; }
};

struct UnkDecalSourceBase_l1a080c {
    virtual void vf_0x0();
    u8 pad_0x8[0x18];
};
struct UnkDecalSourceUser_l1a080c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual s32 vf_0x20();
};
struct UnkDecalSource_l1a080c : UnkDecalSourceBase_l1a080c, UnkDecalSourceUser_l1a080c {};

struct UnkDecalMaterial_l1a080c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    u8 pad_0x8[0x328];
    UnkDecalBindingList_l1a080c bindings;  // +0x330
    u8 pad_0x340[0x18];
    UnkDecalPassList_l1a080c passes;  // +0x358
    u8 pad_0x368[0x28];
    UnkDecalSource_l1a080c* source;  // +0x390
};
static_assert(__builtin_offsetof(UnkDecalMaterial_l1a080c, bindings) == 0x330, "");
static_assert(__builtin_offsetof(UnkDecalMaterial_l1a080c, passes) == 0x358, "");
static_assert(__builtin_offsetof(UnkDecalMaterial_l1a080c, source) == 0x390, "");

struct UnkDecalSlot_l1a080c {
    const void* key;
    u32 value;

    UnkDecalSlot_l1a080c() : key(nullptr), value(0) {}
};
static_assert(sizeof(UnkDecalSlot_l1a080c) == 0x10, "");

struct UnkDecalMgr_l1a080c {
    u8 pad_0x0[0x48];
    u8 pool[0x28];   // +0x48
    u8 stats[0x28];  // +0x70
    u64 released;    // +0x98
    u8 pad_0xa0[0x1c];
    s32 field_0xbc;
    s32 in_use;  // +0xc0
    u8 pad_0xc4[4];
    UnkDecalSlot_l1a080c slots[4][256];  // +0xc8
};
static_assert(__builtin_offsetof(UnkDecalMgr_l1a080c, slots) == 0xc8, "");

static inline void FreeSlot(UnkDecalSlot_l1a080c (&slots)[256], const void* key) {
    for (UnkDecalSlot_l1a080c* slot = slots; slot != slots + 256; ++slot) {
        if (slot->key == key) {
            *slot = UnkDecalSlot_l1a080c();
            return;
        }
    }
}

static inline void FreeSlots(UnkDecalMgr_l1a080c* mgr, UnkDecalBindingList_l1a080c& bindings) {
    for (UnkDecalBinding_l1a080c* b = bindings.begin(); b != bindings.end(); ++b) {
        if (b->type != 'f')
            continue;
        switch (b->level) {
        case 1:
            FreeSlot(mgr->slots[3], b->key);
        case 2:
            FreeSlot(mgr->slots[2], b->key);
        case 3:
            FreeSlot(mgr->slots[1], b->key);
        case 4:
            FreeSlot(mgr->slots[0], b->key);
        }
    }
}

void fn_71001a080c(UnkDecalResourceMgr* mgr_, UnkDecalResource* resource) {
    if (!resource)
        return;
    UnkDecalMgr_l1a080c* mgr = reinterpret_cast<UnkDecalMgr_l1a080c*>(mgr_);
    UnkDecalMaterial_l1a080c* material = reinterpret_cast<UnkDecalMaterial_l1a080c*>(resource);

    FreeSlots(mgr, material->bindings);
    for (UnkDecalPass_l1a080c* pass = material->passes.begin(); pass != material->passes.end(); ++pass)
        FreeSlots(mgr, pass->bindings);

    if (material->source->vf_0x20() == 1)
        mgr->field_0xbc--;
    SAlpha_UnregisterAndClearMaterial(reinterpret_cast<UnkMaterialReset_ovb0098*>(material));
    material->vf_0x8();

    UnkVoicePool_b56* pool = reinterpret_cast<UnkVoicePool_b56*>(mgr->pool);
    UnkSoundVoice2_b56* item = reinterpret_cast<UnkSoundVoice2_b56*>(material);
    u64 index = fn_7100083854(pool, item);
    fn_71001a1020(reinterpret_cast<UnkDecalStats_l1a080c*>(mgr->stats), index).released++;
    fn_7100083834(pool, item);
    mgr->released++;
    mgr->in_use--;
}
