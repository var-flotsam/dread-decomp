// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): the beam's update. After the base movement update
// and fn_71008191e4 it aims along the owner's current bone (fn_710034f54c), runs the state machine at +0x480
// (switching to the pending state at most ten times), and when the beam is in state 4 and the aim turned by
// more than a quarter of pi it asks for state 1. Names and layouts are placeholders.
#include <math.h>

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkMovementComponent.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkBeamVec_ovb0138;
struct UnkGrappleBeamPos_ovb0047;
struct UnkGrappleBeam_l818cf0;
void fn_710034f54c(void* object, u32 id, UnkBeamVec_ovb0138* dir);
void fn_71008191e4(UnkGrappleBeamPos_ovb0047* self);
void fn_71008194e8(UnkGrappleBeam_l818cf0* self);

struct UnkGrappleState_l818cf0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // update
    u64 field_0x8;
    u32 id;  // 0x10
};

typedef UnkGrappleState_l818cf0* __attribute__((aligned(4))) UnkGrappleStatePtr_l818cf0;

// The beam's state machine: states 0 to 4, 5 for none.
struct UnkGrappleMachine_l818cf0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkGrappleState_l818cf0* previous, UnkGrappleState_l818cf0* next);  // entered
    virtual void vf_0x28(UnkGrappleState_l818cf0* current, UnkGrappleState_l818cf0* next);   // left

    u64 field_0x8;
    UnkGrappleState_l818cf0* current;   // 0x10
    UnkGrappleState_l818cf0* previous;  // 0x18
    u32 count;                          // 0x20
    UnkGrappleStatePtr_l818cf0 states[16];  // 0x24
    u32 pending;                        // 0xa4
    bool busy;                          // 0xa8

    // Leaves the current state for state `id` (none when it is 0 and missing) and clears the pending id.
    inline void Switch(u32 id) {
        UnkGrappleState_l818cf0* from = current;
        if (!from || from->id != id) {
            UnkGrappleState_l818cf0* found = FindStateById(states, count, id);
            bool enter = found != nullptr;
            if (enter || id == 0) {
                UnkGrappleState_l818cf0* next = enter ? found : nullptr;
                if (from) {
                    from->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = found;
                if (enter) {
                    next->vf_0x10(previous ? previous->id : 5);
                    vf_0x20(previous, next);
                }
            }
        }
        pending = 5;
    }

    inline void Request(u32 id) {
        pending = id;
        if (id > 4 || busy)
            return;
        Switch(id);
    }

    inline void Update(f32 dt) {
        busy = true;
        if (pending <= 4)
            Switch(pending);
        for (u32 steps = 0; current;) {
            current->vf_0x20(dt);
            if (pending == 5)
                break;
            if (pending <= 4)
                Switch(pending);
            if (++steps == 10)
                break;
        }
        busy = false;
    }
};
static_assert(__builtin_offsetof(UnkGrappleMachine_l818cf0, pending) == 0xa4, "UnkGrappleMachine_l818cf0 layout");

struct UnkGrappleSamusAnimation_l818cf0 : UnkReflObject {
    u8 pad_0x8[0x530 - 0x8];
    u32 bone;  // 0x530
};

struct UnkGrappleOwner_l818cf0 {
    u8 pad_0x0[0x220];
    UnkReflObject* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    void* skeleton;  // 0x2b0
};

struct UnkGrappleLink_l818cf0 {
    u8 pad_0x0[0x10];
    UnkGrappleOwner_l818cf0* owner;  // 0x10
};

struct UnkGrappleBeam_l818cf0 {
    u8 pad_0x0[0x40];
    UnkVec3f aim;  // 0x40
    u8 pad_0x4c[0x230 - 0x4c];
    UnkGrappleLink_l818cf0* link;  // 0x230
    u8 pad_0x238[0x480 - 0x238];
    UnkGrappleMachine_l818cf0 machine;  // 0x480
};
static_assert(__builtin_offsetof(UnkGrappleBeam_l818cf0, machine) == 0x480, "UnkGrappleBeam_l818cf0 layout");

void fn_7100818cf0(UnkGrappleBeam_l818cf0* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    fn_71008191e4(reinterpret_cast<UnkGrappleBeamPos_ovb0047*>(self));
    UnkVec3f old_aim(self->aim.x, self->aim.y, self->aim.z);

    UnkVec3f dir(1.0f, 0.0f, 0.0f);
    UnkGrappleOwner_l818cf0* owner = self->link->owner;
    void* skeleton = owner->skeleton;
    UnkReflObject* animation = owner->animation;
    u32 bone = 0;
    if (animation && fn_710009d50c(animation->vf_0x0(), fn_7100713bb0()))
        bone = static_cast<UnkGrappleSamusAnimation_l818cf0*>(animation)->bone;
    fn_710034f54c(skeleton, bone, reinterpret_cast<UnkBeamVec_ovb0138*>(&dir));
    self->aim = dir;

    UnkGrappleMachine_l818cf0* machine = &self->machine;
    machine->Update(dt);
    if (machine->pending == 5 && machine->current && machine->current->id == 1)
        return;
    fn_71008194e8(self);
    if (!machine->current || machine->current->id != 4)
        return;
    f32 turn = Dot(Normalized(old_aim), Normalized(self->aim));
    turn = !(turn >= 1.0f) ? turn : 1.0f;
    turn = !(turn < -1.0f) ? turn : -1.0f;
    if (acosf(turn) > 0.785398185f)
        machine->Request(1);
}
