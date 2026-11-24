// CKraidAIComponent unit: lets go of what Kraid holds: stops the sounds, ends a pending hold (vtable slot
// 0x210), and, while Kraid is alive, sends the target's movement state of kind 1 "magnetwallrelaxshot".
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkAirState_B71a6;
void CSamusAirMovementState_ForwardRequestIfHasMagnetGlove(UnkAirState_B71a6* self, void*, const u64* value);

struct UnkKraidLife_l5fcea4 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70: 2 when dead
};

struct UnkKraidMoveState_l5fcea4 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
};

#pragma pack(push, 4)
struct UnkKraidTargetMove_l5fcea4 {
    u8 pad_0x0[0x280];
    u8 field_0x280[0x8];  // an object, passed by address
    u8 pad_0x288[0x2d18 - 0x288];
    u32 state_count;
    UnkKraidMoveState_l5fcea4* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkKraidAI_l5fcea4;
struct UnkKraidAIVtbl_l5fcea4 {
    void* slots[0x210 / 8];
    void (*vf_0x210)(UnkKraidAI_l5fcea4* self, void* held);
};

struct UnkKraidAI_l5fcea4 {
    UnkKraidAIVtbl_l5fcea4* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x149d5 - 0x38];
    bool field_0x149d5;
    bool holding;  // 0x149d6
    u8 pad_0x149d7[0x1];
    void* held;  // 0x149d8
};
static_assert(__builtin_offsetof(UnkKraidAI_l5fcea4, held) == 0x149d8, "layout");

// The state of the given kind, or null.
static inline UnkKraidMoveState_l5fcea4* FindState(UnkKraidTargetMove_l5fcea4* move, s32 kind) {
    for (u64 i = 0; i < move->state_count; ++i) {
        UnkKraidMoveState_l5fcea4* state = move->states[i];
        if (state->kind == kind)
            return state;
    }
    return nullptr;
}

void fn_71005fcea4(UnkKraidAI_l5fcea4* self) {
    fn_7100ed78c0(g_unk_71726af0b0);
    self->field_0x149d5 = false;
    if (self->holding && self->held) {
        self->holding = false;
        self->vtable->vf_0x210(self, self->held);
    }
    UnkActor* target = fn_7100d7c990(self->owner);
    auto* life = static_cast<UnkKraidLife_l5fcea4*>(self->owner->field_0x1b8);
    if (!life || !target || life->state == 2)
        return;
    auto* move = static_cast<UnkKraidTargetMove_l5fcea4*>(fn_7100d7c990(self->owner)->field_0x208);
    if (!move)
        return;
    if (UnkKraidMoveState_l5fcea4* state = FindState(move, 1)) {
        u64 id = StringCrc64("magnetwallrelaxshot", 19);
        CSamusAirMovementState_ForwardRequestIfHasMagnetGlove(reinterpret_cast<UnkAirState_B71a6*>(state), move->field_0x280, &id);
    }
}
