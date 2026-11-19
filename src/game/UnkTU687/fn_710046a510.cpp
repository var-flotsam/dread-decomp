// CChozoRobotSoldierUppercutAttack's unit (UnkTU687): starts the attack, remembers where the owner stood
// and, when the owner has a target and its AI component a navigation, hands the navigation a kind-5
// parameter block that faces the side the target is on. Names and layouts are placeholders.
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkFacing_l46a510 {
    f32 x;
    f32 y;
    f32 z;
};

extern const UnkFacing_l46a510 g_unk_71016497a0 __attribute__((visibility("hidden")));
extern const UnkFacing_l46a510 g_unk_71016497ac __attribute__((visibility("hidden")));

// The kind-5 block with a facing at +0x8 and its flag at +0x14.
struct UnkParams5Facing_l46a510 {
    void** vtable;
    UnkFacing_l46a510 facing;  // 0x8
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkUppercutAttack_l46a510 : UnkAttackBase {
    u8 pad_0x20[0x10];
    f32 start_x;  // 0x30
    f32 start_y;  // 0x34
    f32 start_z;  // 0x38
    u8 pad_0x3c[0x4];
    bool field_0x40;  // 0x40
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710046a510(UnkUppercutAttack_l46a510* self) {
    fn_71003608ac(self);
    UnkActor* owner = self->field_0x8;
    self->field_0x40 = false;
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    f32 z = owner->field_0x64;
    self->start_x = x;
    self->start_y = y;
    self->start_z = z;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    UnkAIComponent* ai = static_cast<UnkAIComponent*>(self->field_0x8->field_0x240);
    if (!ai)
        return;
    UnkNavigation* navigation = fn_71010fccf8(ai);
    if (!target || !navigation)
        return;
    UnkParams5Facing_l46a510 params;
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x34 = 5;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    f32 target_x = target->field_0x44;
    params.facing = self->field_0x8->field_0x44 < target_x ? g_unk_71016497a0 : g_unk_71016497ac;
    params.field_0x14 = 1;
    fn_71006f4b0c(navigation, &params, 1);
}
