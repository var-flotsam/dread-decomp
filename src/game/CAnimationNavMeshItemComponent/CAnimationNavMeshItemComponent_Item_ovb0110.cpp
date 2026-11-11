// CAnimationNavMeshItemComponent's unit (tools/tu_map.py unit 1473): the id of the animation the item
// plays, and the navigation-mesh item's destructor (its layout as
// src/game/CNavMeshItemComponent/CNavMeshItemComponent_Containers_ovb0100.cpp constructs it). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkNavMeshAnimObject_ov110 {
    u8 pad_0x0[0xa0];
    CStrId animation;  // 0xa0
};

struct UnkNavMeshAnimDefault_ov110 {
    u8 pad_0x0[0x68];
    CStrId animation;  // 0x68
};

struct UnkNavMeshAnimOwner_ov110;

UnkNavMeshAnimObject_ov110* fn_71009b7f1c(UnkNavMeshAnimOwner_ov110* owner);

struct UnkAnimationNavMeshItem_ov110 {
    u8 pad_0x0[0x30];
    UnkNavMeshAnimOwner_ov110* owner;  // 0x30
    u8 pad_0x38[0x188];
    UnkNavMeshAnimDefault_ov110* fallback;  // 0x1c0
};

// The id the animation lookup returns: starts as the pool's empty entry (making the pool if there is
// none yet) and is released like a CStrId.
struct UnkAnimId_ov110 {
    UnkStringData* data;

    UnkAnimId_ov110() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkAnimId_ov110() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    // Takes a reference to `id`, releasing the one held (CStrId::ReleaseAndTake on a copy).
    void Assign(const CStrId& id) {
        CStrId copy(id);
        reinterpret_cast<CStrId*>(this)->ReleaseAndTake(copy);
    }
};

// The animation the owner's object names (fn_71009b7f1c, its id at +0xa0), else the fallback's
// (+0x1c0, its id at +0x68), else the empty id.
UnkAnimId_ov110 fn_71009b7c9c(UnkAnimationNavMeshItem_ov110* self) {
    UnkAnimId_ov110 result;
    if (UnkNavMeshAnimObject_ov110* object = fn_71009b7f1c(self->owner))
        result.Assign(object->animation);
    else if (UnkNavMeshAnimDefault_ov110* fallback = self->fallback)
        result.Assign(fallback->animation);
    return result;
}

// An element of the array at +0x188 (0x40 bytes, destroyed through its vtable).
struct UnkNavMeshEntry_ov110 {
    void** vtable;
    u8 pad_0x8[0x38];
    void Destroy() { reinterpret_cast<void (*)(UnkNavMeshEntry_ov110*)>(vtable[0])(this); }
};

struct UnkNavMeshFields_ov110;
void fn_71009b81f0(UnkNavMeshFields_ov110* self);

struct UnkNavMeshItemDtor_ov110 {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    u8 fields[0x100];                                     // 0x48, destroyed by fn_71009b81f0
    union {
        UnkOwnedList sources;  // 0x148
    };
    u8 pad_0x170[0x18];
    base::UnkVector<UnkNavMeshEntry_ov110> entries;       // 0x188
};
static_assert(__builtin_offsetof(UnkNavMeshItemDtor_ov110, entries) == 0x188, "layout");

// Destroys the entries (+0x188) and the owned sources (+0x148), then the other fields (fn_71009b81f0)
// and the base component.
void fn_71009b8014(UnkNavMeshItemDtor_ov110* self) {
    self->vtable = g_unk_7101a887d0 + 2;
    self->vtable_0x40 = g_unk_7101a887d0 + 0x33;
    if (self->entries.data) {
        for (u64 i = 0; i < self->entries.count; ++i)
            self->entries.data[i].Destroy();
        self->entries.count = 0;
        self->entries.allocator->vf_0x48(self->entries.data);
        self->entries.capacity = 0;
    }
    self->sources.~UnkOwnedList();
    fn_71009b81f0(reinterpret_cast<UnkNavMeshFields_ov110*>(self->fields));
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
