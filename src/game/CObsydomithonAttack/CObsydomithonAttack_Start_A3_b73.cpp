// CObsydomithonAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac), takes the
// owner's (+0x8) AI component (+0x240) when it is a CObsydomithonAIComponent, sets the pair at +0x50 to (0, 1)
// and hooks the callback fn_710062ae0c (link at +0x30, handle at +0x48) up to the object the owner tracks
// (+0x220), as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp; then sets the pair at +0x58
// to (-1, -1), clears the two flags at +0x60 and calls fn_710062cd74 with 15 on the AI component. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_710062ae0c();
void fn_710062cd74(UnkReflObject* ai, f32 value);

struct UnkObsydomithonOwner_A3_b73 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x18];
    UnkReflObject* field_0x240;
};

struct UnkObsydomithonTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkObsydomithonAttack_A3_b73 {
    u8 pad_0x0[0x8];
    UnkObsydomithonOwner_A3_b73* owner;
    u8 pad_0x10[0x20];
    UnkObsydomithonTrackLink_A3_b73 link;  // 0x30
    UnkWeakHandle* handle;                 // 0x48
    u32 field_0x50;
    u32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    bool field_0x60;
    bool field_0x61;
};

void fn_7100629b14(UnkObsydomithonAttack_A3_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkReflObject* ai = nullptr;
    if (self->owner) {
        ai = self->owner->field_0x240;
        if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710062c51c()))
            ai = nullptr;
    }
    UnkWeakTarget* track = self->owner ? self->owner->field_0x220 : nullptr;
    self->field_0x50 = 0;
    self->field_0x54 = 1;
    if (track) {
        UnkObsydomithonTrackLink_A3_b73* link = &self->link;
        link->context = self;
        link->callback = fn_710062ae0c;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    self->field_0x58 = -1.0f;
    self->field_0x5c = -1.0f;
    self->field_0x60 = false;
    self->field_0x61 = false;
    if (ai)
        fn_710062cd74(ai, 15.0f);
}
