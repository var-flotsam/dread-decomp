// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_7101119fec hands each resolved spawn link's target a
// link to the group's owner, registers the group with the game manager once a target's id is known, then builds
// the group's collider from the named (or the owner's) scene shape. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBatalloonAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU341.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::CStrId;

// The counted reference fn_71003df480 returns (as src/game/CSpawnPointComponent/CSpawnPointComponent_c3_1.cpp).
struct UnkRefObj_c3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object
    s32 refs;
};

struct UnkRef_c3 {
    UnkRefObj_c3* p;
    ~UnkRef_c3() {
        if (p && --p->refs == 0)
            p->vf_0x10();
    }
};

// A link's target as seen here: its id at +0x48.
struct UnkTargetLink_ovb0130;
struct UnkWeakTarget;
void fn_7100a30a9c(UnkTargetLink_ovb0130* self, UnkWeakTarget* target);

struct UnkSpawnTarget_l119fec {
    u8 pad_0x0[0x48];
    CStrId id;  // 0x48
};

struct UnkSpawnLinkHandle_l119fec : base::UnkRefCounted {
    UnkSpawnTarget_l119fec* target;  // 0x10
};

// The base of a link: its callback array (the key the lookup reads) and its state.
struct UnkSpawnLinkBase_l119fec {
    void** vtable;
    UnkErasedVector callbacks;  // 0x8
    u32 state;                  // 0x30: 1 until resolved, then 2

    ~UnkSpawnLinkBase_l119fec() {
        vtable = &g_unk_7101a20998[2];
        if (callbacks.data) {
            for (u64 i = 0; i < callbacks.count; ++i) {
                UnkErasedValue_0x10& v = callbacks.data[i];
                if (v.ops) {
                    v.ops->destroy(&v);
                    v.ops = nullptr;
                }
            }
            callbacks.count = 0;
            callbacks.allocator->vf_0x48(callbacks.data);
            callbacks.capacity = 0;
        }
    }
};

// A link to an object of the scene, resolved on first use (game/CGameLink.h).
struct UnkSpawnLink_l119fec : UnkSpawnLinkBase_l119fec {
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38

    ~UnkSpawnLink_l119fec() {
        vtable = &g_unk_7101a20950[2];
        if (base::UnkRefCounted* p = handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }

    UnkSpawnTarget_l119fec* Get() {
        if (state == 1) {
            {
                UnkRef_c3 ref = fn_71003df480(&callbacks);
                fn_71006eb8cc(&handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        UnkSpawnLinkHandle_l119fec* h = static_cast<UnkSpawnLinkHandle_l119fec*>(handle.ptr);
        return h ? h->target : nullptr;
    }
};
static_assert(sizeof(UnkSpawnLink_l119fec) == 0x40, "UnkSpawnLink_l119fec layout");

void fn_7101127834(UnkSpawnTarget_l119fec* self, UnkSpawnLink_l119fec link);

// A link already resolved to `target` (fn_7100a30a9c constructs it in place).
static inline UnkSpawnLink_l119fec LinkTo(UnkWeakTarget* target) {
    UnkSpawnLink_l119fec link;
    fn_7100a30a9c(reinterpret_cast<UnkTargetLink_ovb0130*>(&link), target);
    return link;
}
bool fn_710119447c(const CStrId& id);
base::reflection::UnkReflObject* fn_71009143d8(void* key);
struct UnkSpawnGroupKey;
extern UnkSpawnGroupKey g_unk_7172a067b8 __attribute__((visibility("hidden")));

struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);
struct UnkStruct_71007e44b8;
struct UnkObj_71007e44b8;
UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const base::global::CStrId* name, u32 index);
struct UnkTriggerCollider_b0058;

struct UnkSpawnGroupCollider_l119fec {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(bool on);
};

// The scene shape a tracker holds.
struct UnkSpawnGroupTracker_l119fec {
    u8 pad_0x0[0x18];
    u64 field_0x18;
    u8 pad_0x20[0x10];
    void* shape;  // 0x30
};

struct UnkSpawnGroupOwner_l119fec {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkSpawnGroup_l119fec {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakTarget* owner_target;  // 0x18
    u8 pad_0x20[0x10];
    UnkSpawnGroupOwner_l119fec* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkSpawnLink_l119fec* links;  // 0x68
    u32 link_count;               // 0x70
    u8 pad_0x74[0xcc];
    bool registered;  // 0x140
    u8 pad_0x141[0x67];
    UnkSpawnGroupCollider_l119fec* collider;  // 0x1a8
    u64 field_0x1b0;
    u8 pad_0x1b8[0x260];
    CStrId shape_name;  // 0x418
};
static_assert(__builtin_offsetof(UnkSpawnGroup_l119fec, registered) == 0x140, "");
static_assert(__builtin_offsetof(UnkSpawnGroup_l119fec, collider) == 0x1a8, "");
static_assert(__builtin_offsetof(UnkSpawnGroup_l119fec, shape_name) == 0x418, "");

static inline UnkStruct_71007e44b8* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return reinterpret_cast<UnkStruct_71007e44b8*>(m);
}

// Has the collision manager create a collider for `owner`, named `name`.
static inline UnkSpawnGroupCollider_l119fec* CreateCollider(UnkStruct_71007e44b8* manager, void* owner,
                                                            CStrId name) {
    return reinterpret_cast<UnkSpawnGroupCollider_l119fec*>(fn_71007e44b8(manager, owner, &name, 0));
}

void fn_7101119fec(UnkSpawnGroup_l119fec* self) {
    for (u64 i = 0; i < self->link_count; ++i) {
        UnkSpawnLink_l119fec& link = self->links[i];
        if (!link.Get())
            continue;
        fn_7101127834(link.Get(), LinkTo(self->owner_target));
        if (fn_710119447c(link.Get()->id) && !self->registered) {
            fn_7100de9540(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172a067b8), reinterpret_cast<UnkWeakTarget*>(self));
            self->registered = true;
        }
    }

    UnkSpawnGroupTracker_l119fec* tracker;
    if (self->shape_name.data == &g_unk_7101d47108->empty)
        tracker = reinterpret_cast<UnkSpawnGroupTracker_l119fec*>(fn_710028aca8(self->owner));
    else
        tracker = reinterpret_cast<UnkSpawnGroupTracker_l119fec*>(fn_71009143d8(&self->shape_name));
    if (!tracker)
        return;
    void* shape = tracker->shape;
    if (!shape) {
        self->collider = nullptr;
        return;
    }
    self->collider = CreateCollider(Manager(), self, CStrId(self->owner->name));
    self->field_0x1b0 = tracker->field_0x18;
    fn_7100d36d30(reinterpret_cast<UnkTriggerCollider_b0058*>(self->collider), shape);
    self->collider->vf_0x30(true);
}
