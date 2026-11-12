// CCoreXAIComponent's unit (tools/tu_map.py unit 759): the destructors (listener at +0x149d8, refcounted
// pointer at +0x149d0, then fn_71003ac388) and the hit filter that blocks plasma hits on the "block_plasma"
// collider. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CBossAIComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a3fc88[];  // the component's vtable

struct UnkCoreXAI_ovb0095;

// A listener holding a weak handle to what it listens to; it unregisters on destruction.
struct UnkCoreXListener_ovb0095 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* link;  // 0x18

    ~UnkCoreXListener_ovb0095() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkWeakHandle* l = link) {
            if (UnkWeakTarget* target = l->target) {
                fn_710117b3f0(target, this);
                l = link;
            }
            if (l && --l->ref_count == 0)
                l->vf_0x10();
        }
    }
};

struct UnkCoreXAI_ovb0095 {
    void** vtable;
    u8 pad_0x8[0x149c8];
    base::UnkRefPtrMember field_0x149d0;
    UnkCoreXListener_ovb0095 listener;  // 0x149d8
};
static_assert(__builtin_offsetof(UnkCoreXAI_ovb0095, listener) == 0x149d8, "layout");

// CCoreXAIComponent's destructor: the listener at +0x149d8 and the handle at +0x149d0, then the base's
// destructor fn_71003ac388.
void fn_71004d9728(UnkCoreXAI_ovb0095* self) {
    self->vtable = &g_unk_7101a3fc88[2];
    self->listener.~UnkCoreXListener_ovb0095();
    self->field_0x149d0.~UnkRefPtrMember();
    fn_71003ac388(self);
}

// The deleting form.
void fn_71004d9800(UnkCoreXAI_ovb0095* self) {
    self->vtable = &g_unk_7101a3fc88[2];
    self->listener.~UnkCoreXListener_ovb0095();
    self->field_0x149d0.~UnkRefPtrMember();
    fn_71003ac388(self);
    operator delete(self);
}

using base::global::CStrId;

// As src/game/CGoliathAIComponent/CGoliathAIComponent_HitFilter_b55.cpp declares it.
struct UnkGoliathAI_b55;
struct UnkGoliathHitInfo_b55;
struct UnkGoliathHitTarget_b55;
struct UnkCollisionContainer;

struct UnkCoreXOwner_ovb0095 {
    u8 pad_0x0[0x1b8];
    void* field_0x1b8;
    u8 pad_0x1c0[0x80];
    void* field_0x240;
    u8 pad_0x248[0x58];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkCoreXHitAI_ovb0095 {
    u8 pad_0x0[0x30];
    UnkCoreXOwner_ovb0095* owner;  // 0x30
};

struct UnkCoreXHitInfo_ovb0095 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

struct UnkCoreXHitTarget_ovb0095 {
    u8 pad_0x0[0x10];
    void* object;
};

// CCoreXAIComponent's hit filter: a hit with info flags 0x5000 on the collider named "block_plasma"
// writes 0 to *out and returns 1 (unless fn_7101161088 refuses the owner's +0x1b8 and the flags have
// 0x08000400); everything else defers to fn_71011061ac.
u32 fn_71004db51c(UnkCoreXHitAI_ovb0095* self, u32* out, void* arg2, UnkCoreXHitInfo_ovb0095* info,
                  UnkCoreXHitTarget_ovb0095** hit) {
    if (info->flags & 0x5000) {
        if (fn_7101161088(self->owner->field_0x1b8) || !(info->flags & 0x08000400)) {
            UnkCoreXOwner_ovb0095* owner = self->owner;
            if (owner->field_0x240 && owner->field_0x2a0) {
                static CStrId s_plasma("block_plasma", true);
                CStrId* id = fn_7100864018(self->owner->field_0x2a0, *hit ? (*hit)->object : nullptr);
                if (id->data == s_plasma.data) {
                    *out = 0;
                    return 1;
                }
            }
        }
    }
    return fn_71011061ac(reinterpret_cast<UnkGoliathAI_b55*>(self), out, arg2,
                         reinterpret_cast<UnkGoliathHitInfo_b55*>(info),
                         reinterpret_cast<UnkGoliathHitTarget_b55**>(hit));
}
