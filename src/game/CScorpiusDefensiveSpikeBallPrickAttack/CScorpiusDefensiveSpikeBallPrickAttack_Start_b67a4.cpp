// CScorpiusDefensiveSpikeBallPrickAttack's unit (tools/tu_map.py, the name from reflection): the
// attack's start. After the base fn_71006865b0 it hooks fn_71006879a4 up to the owner's track (+0x220)
// through the link at +0x38 and the weak handle at +0x50 (the shape of
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp), then sets the value at +0x2318
// of the owner's target (fn_7100d7c990, its +0x208) to 0.5 with its flag at +0x231c. Names and layouts
// are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkPrickAttack_b67a4;
void fn_71006865b0(UnkPrickAttack_b67a4* self);
void fn_71006879a4();

struct UnkPrickLink_b67a4 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkPrickTarget_b67a4 {
    u8 pad_0x0[0x2318];
    f32 value;  // 0x2318
    bool set;  // 0x231c
};

struct UnkPrickAttack_b67a4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x38 - 0x10];
    UnkPrickLink_b67a4 link;  // 0x38
    UnkWeakHandle* handle;  // 0x50
};

void fn_7100687008(UnkPrickAttack_b67a4* self) {
    fn_71006865b0(self);
    UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
    UnkPrickLink_b67a4* link = &self->link;
    link->context = self;
    link->callback = fn_71006879a4;
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
    if (UnkPrickTarget_b67a4* target = static_cast<UnkPrickTarget_b67a4*>(fn_7100d7c990(self->owner)->field_0x208)) {
        target->value = 0.5f;
        target->set = true;
    }
}
