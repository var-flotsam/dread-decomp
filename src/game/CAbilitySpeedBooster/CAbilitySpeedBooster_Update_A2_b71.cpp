// CAbilitySpeedBooster's unit (tools/tu_map.py, the name from reflection): runs the callback of the object at
// +0x3b8 (+0x68 with +0x60), then, when the owner actor (+0x30 of +0xc0) has its +0x200 object, ends the boost
// (fn_710033cd50 with 1) if that object is flagged (+0x7e4), or if its speed (+0x58) points along the facing
// direction (negative with the byte at +0x3a0 set, else positive) while the actor's +0x208 object is neither
// flagged (+0x2325) nor in mode 1 (+0x2d08, +0x10) with its +0x2f48 flag set. Names and layouts are
// placeholders.
#include "game/CAbilitySpeedBooster.h"
#include "types.h"

// As src/game/UnkUnit435/UnkUnit435_OnSameId.cpp declares it.
struct UnkUnit435Self;

struct UnkSpeedBoosterCallback_A2_b71 {
    u8 pad_0x0[0x60];
    void* arg;                // 0x60
    void (*callback)(void*);  // 0x68
};

struct UnkSpeedBoosterMode_A2_b71 {
    u8 pad_0x0[0x10];
    s32 mode;  // 0x10
};

struct UnkSpeedBoosterState_A2_b71 {
    u8 pad_0x0[0x2325];
    bool field_0x2325;
    u8 pad_0x2326[0x9e2];
    UnkSpeedBoosterMode_A2_b71* field_0x2d08;
    u8 pad_0x2d10[0x238];
    bool field_0x2f48;
};

struct UnkSpeedBoosterMovement_A2_b71 {
    u8 pad_0x0[0x58];
    f32 speed;  // 0x58
    u8 pad_0x5c[0x788];
    bool field_0x7e4;
};

struct UnkSpeedBoosterActor_A2_b71 {
    u8 pad_0x0[0x200];
    UnkSpeedBoosterMovement_A2_b71* movement;  // 0x200
    UnkSpeedBoosterState_A2_b71* state;        // 0x208
};

struct UnkSpeedBoosterOwner_A2_b71 {
    u8 pad_0x0[0x30];
    UnkSpeedBoosterActor_A2_b71* actor;  // 0x30
};

struct UnkSpeedBooster_A2_b71 {
    u8 pad_0x0[0xc0];
    UnkSpeedBoosterOwner_A2_b71* owner;  // 0xc0
    u8 pad_0xc8[0x2d8];
    bool field_0x3a0;
    u8 pad_0x3a1[0x17];
    UnkSpeedBoosterCallback_A2_b71* field_0x3b8;
};

void fn_710033d28c(UnkSpeedBooster_A2_b71* self) {
    if (UnkSpeedBoosterCallback_A2_b71* cb = self->field_0x3b8) {
        if (cb->callback)
            cb->callback(cb->arg);
    }
    UnkSpeedBoosterActor_A2_b71* actor = self->owner->actor;
    UnkSpeedBoosterMovement_A2_b71* movement = actor->movement;
    if (!movement)
        return;
    UnkSpeedBoosterState_A2_b71* state = actor->state;
    bool blocked = state && (state->field_0x2325 ||
                             (state->field_0x2d08 && state->field_0x2d08->mode == 1 && state->field_0x2f48));
    bool along = self->field_0x3a0 ? movement->speed < 0.0f : movement->speed > 0.0f;
    if ((!blocked && along) || movement->field_0x7e4)
        fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
}
