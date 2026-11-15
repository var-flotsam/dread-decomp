// CEnemyMovement unit: stops the movement. Runs the base step, unhooks the link at +0x1be8 from the
// weak target and drops the handle, then requests state 0 ("none" 5) from the state machine at +0x838.
// Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCharacterMovementOwner_ovb0021;
void fn_710097973c(UnkCharacterMovementOwner_ovb0021* self);

struct UnkEnemyMovement_l0c91dc {
    u8 pad_0x0[0x838];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x8e8[0x1be8 - 0x8e8];
    u8 link[0x18];  // 0x1be8
    UnkWeakHandle* handle;  // 0x1c00
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c91dc, handle) == 0x1c00, "layout");

void fn_71010c91dc(UnkEnemyMovement_l0c91dc* self) {
    fn_710097973c(reinterpret_cast<UnkCharacterMovementOwner_ovb0021*>(self));
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    RequestState_A5_b73<15, 5>(&self->machine, 0);
}
