// CAbilitySpeedBooster's unit (tools/tu_map.py, the name from reflection): ends the boost (fn_710033cd50 with 1)
// when the game manager (g_unk_7101d47310) is active (+0xc8, or the flag at +0x10c) with the byte at +0xc4 set
// and the one at +0xc6 clear; otherwise, with the owner actor's (+0x30 of +0xc0) +0x208 object, unless it is in
// mode 1 or 3 (+0x2d08, +0x10); without it, when the +0x210 object's +0x118 is at most 100 or the +0x200
// object's speed (+0x10c) points along the facing direction (negative with the byte at +0x3a0 set, else
// positive). Names and layouts are placeholders.
#include "game/CAbilitySpeedBooster.h"
#include "game/UnkGameManager.h"
#include "types.h"

// As src/game/UnkUnit435/UnkUnit435_OnSameId.cpp declares it.
struct UnkUnit435Self;

struct UnkSpeedBoosterMode2_A2_b71 {
    u8 pad_0x0[0x10];
    s32 mode;  // 0x10
};

struct UnkSpeedBoosterState2_A2_b71 {
    u8 pad_0x0[0x2d08];
    UnkSpeedBoosterMode2_A2_b71* field_0x2d08;
};

struct UnkSpeedBoosterMovement2_A2_b71 {
    u8 pad_0x0[0x10c];
    f32 speed;  // 0x10c
};

struct UnkSpeedBoosterGauge_A2_b71 {
    u8 pad_0x0[0x118];
    f32 value;  // 0x118
};

struct UnkSpeedBoosterActor2_A2_b71 {
    u8 pad_0x0[0x200];
    UnkSpeedBoosterMovement2_A2_b71* movement;  // 0x200
    UnkSpeedBoosterState2_A2_b71* state;        // 0x208
    UnkSpeedBoosterGauge_A2_b71* gauge;         // 0x210
};

struct UnkSpeedBoosterOwner2_A2_b71 {
    u8 pad_0x0[0x30];
    UnkSpeedBoosterActor2_A2_b71* actor;  // 0x30
};

struct UnkSpeedBooster2_A2_b71 {
    u8 pad_0x0[0xc0];
    UnkSpeedBoosterOwner2_A2_b71* owner;  // 0xc0
    u8 pad_0xc8[0x2d8];
    bool field_0x3a0;
};

void fn_710033e348(UnkSpeedBooster2_A2_b71* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    const u8* bytes = reinterpret_cast<const u8*>(manager);
    if ((manager->field_0xc8 != 0 || manager->field_0x10c) && bytes[0xc4] && !bytes[0xc6]) {
        fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
        return;
    }
    UnkSpeedBoosterActor2_A2_b71* actor = self->owner->actor;
    if (UnkSpeedBoosterState2_A2_b71* state = actor->state) {
        UnkSpeedBoosterMode2_A2_b71* mode = state->field_0x2d08;
        if (mode && (mode->mode == 1 || mode->mode == 3))
            return;
        fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
        return;
    }
    if (!actor || !actor->gauge)
        return;
    if (actor->gauge->value <= 100.0f) {
        fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
        return;
    }
    UnkSpeedBoosterMovement2_A2_b71* movement = actor->movement;
    if (!movement)
        return;
    f32 speed = movement->speed;
    bool along = self->field_0x3a0 ? speed < 0.0f : speed > 0.0f;
    if (along)
        fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
}
