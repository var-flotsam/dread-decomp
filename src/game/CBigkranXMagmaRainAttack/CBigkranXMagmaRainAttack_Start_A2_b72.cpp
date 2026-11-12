// CBigkranXMagmaRainAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac) sets
// the two words at +0x30 to 1; with an owner (+0x8), hooks the callback fn_71003eb3cc (link at +0x38,
// handle at +0x50) up to the object it tracks (+0x220), as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp, and gives its AI (+0x240) value 1
// (fn_710111094c); always resets the pair at +0x58 to -1. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_710111094c(UnkReflObject* ai, u32 value);
void fn_71003eb3cc();

struct UnkMagmaRainLink_A2_b72 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkMagmaRainOwner_A2_b72 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
    u8 pad_0x228[0x18];
    UnkReflObject* field_0x240;
};

struct UnkMagmaRainAttack_A2_b72 {
    u8 pad_0x0[0x8];
    UnkMagmaRainOwner_A2_b72* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u32 field_0x34;
    UnkMagmaRainLink_A2_b72 link;  // 0x38
    UnkWeakHandle* handle;         // 0x50
    f32 field_0x58;
    f32 field_0x5c;
};

void fn_71003ea5fc(UnkMagmaRainAttack_A2_b72* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 1;
    self->field_0x34 = 1;
    if (UnkMagmaRainOwner_A2_b72* owner = self->owner) {
        UnkWeakTarget* track = owner->field_0x220;
        UnkReflObject* ai = owner->field_0x240;
        if (track) {
            UnkMagmaRainLink_A2_b72* link = &self->link;
            link->context = self;
            link->callback = fn_71003eb3cc;
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
        if (ai)
            fn_710111094c(ai, 1);
    }
    self->field_0x58 = -1.0f;
    self->field_0x5c = -1.0f;
}
