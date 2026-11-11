#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CSamusFloorSlideMovementState/CParams.h"
#include "types.h"

struct UnkFlagged;
struct UnkShape;

extern u64 g_unk_717273caf0 __attribute__((visibility("hidden")));

struct UnkMovementShape {
    u8 unk_0x0[0x20];
    u64 field_0x20;
};

struct CSamusMovementState {
    u8 unk_0x0[0x38];
    UnkFlagged* owner;
};

// Whether the owner's shape no longer carries the default id g_unk_717273caf0.
bool fn_71009b2d0c(CSamusMovementState* self) {
    UnkMovementShape* shape = reinterpret_cast<UnkMovementShape*>(fn_7100976588(self->owner));
    return shape->field_0x20 != g_unk_717273caf0;
}

struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

struct UnkMovementRequest {
    u8 unk_0x0[0x8];
    s64 id;  // 0x8; -1 when unset
};

struct UnkMovementEntity {
    u8 unk_0x0[0x220];
    UnkStruct_7101176ea8* animation;
};

struct UnkMovementTransitions;

struct CSamusMovementState2 {
    u8 unk_0x0[0x30];
    UnkMovementEntity* entity;
    u8 unk_0x38[0x10];
    u8 transitions[0x100];  // 0x48
    u32 field_0x148;
    u8 unk_0x14c[0x4];
    UnkMovementRequest* request;  // 0x150
    bool field_0x158;
};

// Leaves the state: clears it, stops its animation request if one is running, and moves to state 15.
void fn_71009b0c74(CSamusMovementState2* self) {
    UnkMovementRequest* request = self->request;
    self->field_0x158 = false;
    self->field_0x148 = 0;
    if (request && request->id != -1) {
        if (UnkStruct_7101176ea8* animation = self->entity->animation)
            fn_7101176ea8(animation, reinterpret_cast<const UnkRequest_7101176ea8*>(&request->id), 0xf4061ff25a2163d6);
    }
    u32 next = 15;
    fn_7100997128(reinterpret_cast<UnkMovementTransitions*>(self->transitions), &next);
}

struct CSamusMovementState3 {
    void** vtable;
    u8 unk_0x8[0x159 - 0x8];
    bool field_0x159;
};

// When the pending flag at 0x159 is set, runs slot 0x140 with true and clears the flag.
bool fn_71009b16c0(CSamusMovementState3* self) {
    if (!self->field_0x159)
        return false;
    bool result = reinterpret_cast<bool (*)(CSamusMovementState3*, bool)>(self->vtable[0x140 / 8])(self, true);
    self->field_0x159 = false;
    return result;
}
