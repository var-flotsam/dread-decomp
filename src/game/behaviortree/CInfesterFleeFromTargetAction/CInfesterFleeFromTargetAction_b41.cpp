// behaviortree::CInfesterFleeFromTargetAction unit. fn_71005c804c: the action's start; after the base
// start (fn_7100394bd0) it keeps the owner's AI when it is a CInfesterAIComponent (+0x30) and its
// navigation (+0x38), resets its flags and timers, and either marks the action done (+0x5b) when the
// owner's CEnemyMovement is in state 4 or starts fleeing (fn_71005c8160). Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAIComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68*);
u32 fn_71010cc3a4(void*);

struct UnkOwner_if41 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x48];
    UnkReflObject* ai;  // 0x240
};

struct UnkSelf_if41 {
    u8 pad_0x0[0x18];
    UnkOwner_if41* owner;
    u8 pad_0x20[0x10];
    UnkReflObject* ai;          // 0x30
    UnkNavigation* navigation;  // 0x38
    u8 pad_0x40[0x19];
    u8 field_0x59;
    u8 field_0x5a;
    u8 done;  // 0x5b
    u8 pad_0x5c[2];
    u8 field_0x5e;
    u8 pad_0x5f;
    f32 field_0x60;
    f32 field_0x64;
    u8 field_0x68;
};

void fn_71005c8160(UnkSelf_if41* self, u32 arg);

void fn_71005c804c(UnkSelf_if41* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* ai = self->owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71005ca678())) {
        self->ai = nullptr;
        return;
    }
    self->ai = ai;
    self->navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    if (!self->navigation)
        return;
    self->field_0x60 = 0.0f;
    self->field_0x64 = 0.3f;
    self->field_0x5e = 0;
    self->field_0x59 = 0;
    self->field_0x5a = 0;
    self->done = 0;
    self->field_0x68 = 0;
    UnkReflObject* movement = self->owner->movement;
    if (movement && !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()))
        movement = nullptr;
    if (fn_71010cc3a4(movement) == 4) {
        self->done = 1;
        return;
    }
    fn_71005c8160(self, 0);
}
