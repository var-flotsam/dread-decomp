// CAbilitySpeedBooster unit (tools/tu_map.py, the name from reflection): for a positive amount while the state
// at +0x3b8 is in mode 2 or 3 and the owner actor's (+0x30 of +0xc0) +0x210 object is present with its +0x3a8
// flag clear, ends the boost (fn_710033cd50 with 1) when the input points against the facing direction (the
// constant at 0x7101647920, or 0x71016478d0 when the byte at +0x3a0 is set). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

// As src/game/UnkUnit435/UnkUnit435_OnSameId.cpp declares it.
struct UnkUnit435Self;
void fn_710033cd50(UnkUnit435Self* self, u32 arg);

extern const CVector2D g_unk_71016478d0 __attribute__((visibility("hidden")));
extern const CVector2D g_unk_7101647920 __attribute__((visibility("hidden")));

struct UnkSpeedBoosterMovement_b67a2 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkSpeedBoosterActor2_b67a2 {
    u8 pad_0x0[0x210];
    UnkSpeedBoosterMovement_b67a2* movement;
};

struct UnkSpeedBoosterOwner2_b67a2 {
    u8 pad_0x0[0x30];
    UnkSpeedBoosterActor2_b67a2* actor;
};

struct UnkSpeedBoosterState2_b67a2 {
    u8 pad_0x0[0x10];
    u32 mode;
};

struct UnkSpeedBooster2_b67a2 {
    u8 pad_0x0[0xc0];
    UnkSpeedBoosterOwner2_b67a2* owner;
    u8 pad_0xc8[0x3a0 - 0xc8];
    bool field_0x3a0;
    u8 pad_0x3a1[0x17];
    UnkSpeedBoosterState2_b67a2* state;
};

void fn_710033e2c8(UnkSpeedBooster2_b67a2* self, f32 amount, const CVector2D* input) {
    if (!(amount > 0.0f))
        return;
    UnkSpeedBoosterState2_b67a2* state = self->state;
    if (state == nullptr || (state->mode & ~1u) != 2)
        return;
    UnkSpeedBoosterActor2_b67a2* actor = self->owner->actor;
    if (actor == nullptr || actor->movement == nullptr || actor->movement->field_0x3a8)
        return;
    const CVector2D& facing = self->field_0x3a0 ? g_unk_71016478d0 : g_unk_7101647920;
    if (facing.x * input->x + facing.y * input->y < 0.0f)
        fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
}
