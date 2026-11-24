// CBillboardCollisionComponent's unit (tools/tu_map.py unit 1151): the (re)initialisation. After the base
// step it empties the pointer array at +0x168 to sixteen zeroed slots and makes room for sixteen entries in
// the keyed table at +0x140, builds one entry per lock-on point ("lockon0" to "lockon15", through the
// component's virtual slot 0x168) into the array, sizes the flag array at +0x190 to the count the owner's
// definition gives and registers the component's link with the sub-area manager. Names and layouts are
// placeholders.
#include <new>
#include <stdio.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU883.h"
#include "types.h"

using base::global::CStrId;

// The game's id table, built on first use; its slot at +0x9f0 is the one the entries are given.
struct UnkBbIds_l723f50 {
    u8 pad_0x0[0x9f0];
    CStrId field_0x9f0;
    u8 pad_0x9f8[0x2578 - 0x9f8];
    UnkBbIds_l723f50() asm("fn_7100031894");
    ~UnkBbIds_l723f50() asm("fn_71000407f0");
};

// Two arrays that share a count: the keys at +0x0 and the values at +0x8.
struct UnkBbTable_l723f50 {
    u64* keys;
    u64* values;
    u32 count;     // 0x10
    u32 capacity;  // 0x14
    u32 step;      // 0x18
    u32 flags;     // 0x1c
    base::UnkAllocator* allocator;  // 0x20

    void Reserve(u32 n) {
        u64* new_keys = static_cast<u64*>(allocator->vf_0x40(n * sizeof(u64), 8, flags));
        u64* new_values = static_cast<u64*>(allocator->vf_0x40(n * sizeof(u64), 8, flags));
        if (keys) {
            __builtin_memcpy(new_keys, keys, count * sizeof(u64));
            allocator->vf_0x48(keys);
        }
        if (values) {
            __builtin_memcpy(new_values, values, count * sizeof(u64));
            allocator->vf_0x48(values);
        }
        keys = new_keys;
        values = new_values;
        capacity = n;
    }
};

struct UnkBbDefinition_l723f50 {
    u8 pad_0x0[0x110];
    u32 field_0x110;
};

struct UnkBbOwner_l723f50 {
    u8 pad_0x0[0x10];
    UnkBbDefinition_l723f50* definition;  // 0x10
};

struct UnkBillboardCollision_l723f50 {
    void** vtable;
    u8 pad_0x8[0x128];
    UnkBbOwner_l723f50* owner;  // 0x130
    u8 pad_0x138[8];
    UnkBbTable_l723f50 table;  // 0x140
    base::global::CRntVector<void*> lockons;  // 0x168
    base::global::CRntVector<u32> flags;      // 0x190
    void* link;                               // 0x1b8
};
static_assert(__builtin_offsetof(UnkBillboardCollision_l723f50, lockons) == 0x168, "layout");
static_assert(__builtin_offsetof(UnkBillboardCollision_l723f50, flags) == 0x190, "layout");
static_assert(__builtin_offsetof(UnkBillboardCollision_l723f50, link) == 0x1b8, "layout");

void fn_71008614c0(UnkBillboardCollision_l723f50* self);

inline const UnkBbIds_l723f50& Ids() {
    static UnkBbIds_l723f50 s_ids asm("g_unk_7101d552d0");
    return s_ids;
}

using UnkBbBuild_l723f50 = void* (*)(UnkBillboardCollision_l723f50*, const CStrId&, const CStrId&, const CStrId&,
                                     const CStrId&, u32, u32, u32, void*, s32, void*, s32, void*, s32, bool,
                                     bool, bool);

void fn_7100723f50(UnkBillboardCollision_l723f50* self) {
    fn_71008614c0(self);
    self->lockons.count = 0;
    self->lockons.Resize(16);
    self->table.count = 0;
    if (self->table.capacity < 16)
        self->table.Reserve(16);
    for (u32 i = 0; i < 16; ++i) {
        char name[16];
        sprintf(name, "lockon%d", i);
        CStrId id(name, true);
        const CStrId& slot = Ids().field_0x9f0;
        void* entry = reinterpret_cast<UnkBbBuild_l723f50>(self->vtable[0x168 / 8])(
            self, id, id, CStrId::Empty(), slot, 0, 1, 0, nullptr, -1, nullptr, -1, nullptr, -1, false,
            false, true);
        self->lockons.PushBack(entry);
    }
    self->flags.Resize(self->owner->definition->field_0x110);
    UnkManager* manager = g_unk_7101d47360;
    if (!manager) {
        manager = static_cast<UnkManager*>(fn_71000839a0(0xae8, 0x3f));
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    if (self->link) {
        fn_71005c4828(self->link, &self->link);
        self->link = nullptr;
    }
    self->link = reinterpret_cast<char*>(manager) + 0xa60;
    fn_71005c41bc(self->link, reinterpret_cast<UnkCallback_ac3184*>(&self->link));
}
