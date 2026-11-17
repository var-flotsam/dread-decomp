// Unit 2689 (the player movement): fn_71010d9f88 starts the hold state (+0x18a4): clears its timers, plays two
// timeline ids on the owner, locks the owner's input, saves the current movement state (+0x284) into +0x338
// and adds the unit's key to g_unk_71726af0a8. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CInputComponent.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;

struct UnkPlayerMovementD_b0204;
struct UnkDoorKeyOwner_dae8a4;
void fn_7100dae8a4(UnkDoorKeyOwner_dae8a4* self, bool add, CStrId* key);
extern const u64 g_unk_710164cef8;  // reached through the GOT
extern const u64 g_unk_710164cf00;  // reached through the GOT
extern CStrId g_unk_7172a03e18 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a03e40 __attribute__((visibility("hidden")));

struct UnkHoldOwner_l0d9f88 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x108];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkVec2_l0d9f88 {
    f32 x, y;
};

struct UnkVec3_l0d9f88 {
    f32 x, y, z;
};

// The movement state the hold saves and later restores.
struct UnkMoveState_l0d9f88 {
    UnkVec3_l0d9f88 field_0x0;
    UnkVec3_l0d9f88 field_0xc;
    UnkVec2_l0d9f88 field_0x18;
    UnkVec3_l0d9f88 field_0x20;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    UnkVec2_l0d9f88 field_0x40;
    f32 field_0x48;
    UnkVec2_l0d9f88 field_0x4c;
    f32 field_0x54;
    f32 field_0x58;
};
static_assert(sizeof(UnkMoveState_l0d9f88) == 0x5c, "layout");

struct UnkHoldMovement_l0d9f88 {
    u8 pad_0x0[0x30];
    UnkHoldOwner_l0d9f88* owner;  // 0x30
    u8 pad_0x38[0x24c];
    UnkMoveState_l0d9f88 current;  // 0x284
    u8 pad_0x2e0[0x58];
    UnkMoveState_l0d9f88 saved;  // 0x338
    u8 pad_0x394[0x1510];
    bool holding;  // 0x18a4
    u8 pad_0x18a5[0x3];
    f32 field_0x18a8;
    f32 field_0x18ac;
};
static_assert(__builtin_offsetof(UnkHoldMovement_l0d9f88, saved) == 0x338, "layout");
static_assert(__builtin_offsetof(UnkHoldMovement_l0d9f88, holding) == 0x18a4, "layout");

void fn_71010d9f88(UnkPlayerMovementD_b0204* movement) {
    auto* self = reinterpret_cast<UnkHoldMovement_l0d9f88*>(movement);
    if (self->holding)
        return;
    self->holding = true;
    self->field_0x18a8 = 0.0f;
    self->field_0x18ac = 0.0f;
    // At function scope: neither shares its stack slot, as in the original.
    base::reflection::UnkReflTypeIdArg mask;
    base::reflection::UnkReflTypeIdArg mask2;
    auto playTimeline = [](UnkTimelineComponent* timeline, const u64* id, base::reflection::UnkReflTypeIdArg& value) {
        value.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, id, &value.id, false, -1.0f);
    };
    if (self->owner->timeline) {
        playTimeline(self->owner->timeline, &g_unk_710164cef8, mask);
        playTimeline(self->owner->timeline, &g_unk_710164cf00, mask2);
    }
    if (UnkInputComponent* input = self->owner->input)
        fn_7100872460(input, true, false, &g_unk_7172a03e18);
    self->saved.field_0x0 = self->current.field_0x0;
    self->saved.field_0xc = self->current.field_0xc;
    self->saved.field_0x18 = self->current.field_0x18;
    self->saved.field_0x20 = self->current.field_0x20;
    self->saved.field_0x2c = self->current.field_0x2c;
    self->saved.field_0x30 = self->current.field_0x30;
    self->saved.field_0x34 = self->current.field_0x34;
    self->saved.field_0x38 = self->current.field_0x38;
    self->saved.field_0x3c = self->current.field_0x3c;
    self->saved.field_0x40 = self->current.field_0x40;
    self->saved.field_0x48 = self->current.field_0x48;
    self->saved.field_0x4c = self->current.field_0x4c;
    self->saved.field_0x54 = self->current.field_0x54;
    self->saved.field_0x58 = self->current.field_0x58;
    fn_7100dae8a4(reinterpret_cast<UnkDoorKeyOwner_dae8a4*>(g_unk_71726af0a8), true, &g_unk_7172a03e40);
}
