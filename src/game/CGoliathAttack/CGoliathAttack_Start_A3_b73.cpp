// CGoliathAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac) sets +0x28 to
// 1, calls fn_710111094c with 1 on the owner's (+0x8) AI component (+0x240), hooks the callback fn_710058f630
// (link at +0x48, handle at +0x60) up to the object the owner tracks (+0x220), as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp, and copies the sub-area manager's
// (g_unk_7101d47360, created on first use) id at +0x3f0, or the empty id, to +0x40. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_710058f630();

struct UnkManager;
extern UnkManager* g_unk_7101d47360;  // the sub-area manager

struct UnkGoliathManager_A3_b73 {
    u8 pad_0x0[0x3f0];
    CStrId* field_0x3f0;
};

struct UnkGoliathOwner_A3_b73 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x18];
    UnkReflObject* field_0x240;
};

struct UnkGoliathTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkGoliathAttack_A3_b73 {
    u8 pad_0x0[0x8];
    UnkGoliathOwner_A3_b73* owner;
    u8 pad_0x10[0x18];
    u32 field_0x28;
    u8 pad_0x2c[0x14];
    CStrId id;                         // 0x40
    UnkGoliathTrackLink_A3_b73 link;   // 0x48
    UnkWeakHandle* handle;             // 0x60
};

void fn_710058e67c(UnkGoliathAttack_A3_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x28 = 1;
    fn_710111094c(self->owner->field_0x240, 1);
    UnkGoliathTrackLink_A3_b73* link = &self->link;
    link->context = self;
    link->callback = fn_710058f630;
    UnkWeakTarget* track = self->owner->field_0x220;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    if (!g_unk_7101d47360) {
        void* p = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(p);
        g_unk_7101d47360 = static_cast<UnkManager*>(p);
    }
    UnkGoliathManager_A3_b73* manager = reinterpret_cast<UnkGoliathManager_A3_b73*>(g_unk_7101d47360);
    CStrId* id = manager->field_0x3f0;
    self->id = id ? *id : g_unk_7101d470e0;
}
