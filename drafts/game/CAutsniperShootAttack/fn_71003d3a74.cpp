// CAutsniperShootAttack's unit (tools/tu_map.py unit 576): the attack's start. After the attack base's step,
// moves to state 1, reads the shot's model answer, the owner's facing and its target's position, resets the
// aim, plays the shooting layers on the owner's character class and sends it a zero, tells an autsniper AI
// it shoots, then enters state 1 (fn_71003d3cf8) and records whether the owner's movement sits on a wall.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::reflection::CClass;

struct UnkActorTarget;
struct UnkGrappleAnswer_b50;
struct UnkAutsniperSend_b58;

extern u8 g_unk_71016492a8;  // reached through the GOT

void fn_71003d3cf8(UnkAutsniperSend_b58* self);

struct UnkAutsniperAI_l3d3a74 {
    virtual const CClass* vf_0x0();
};
void fn_71003daedc(UnkAutsniperAI_l3d3a74* ai, bool shooting);

struct UnkAutsniperMovement_l3d3a74 {
    u8 pad_0x0[0x971];
    u8 field_0x971;
    u8 pad_0x972[0xa0c - 0x972];
    u8 field_0xa0c;
};

struct UnkAutsniperOwner_l3d3a74 {
    u8 pad_0x0[0x38];
    u8 transform[0xc];  // 0x38
    u8 pad_0x44[0x218 - 0x44];
    UnkAutsniperMovement_l3d3a74* movement;  // 0x218
    UnkActorTarget* character;               // 0x220
    void* model;                             // 0x228
    u8 pad_0x230[0x10];
    UnkAutsniperAI_l3d3a74* ai;  // 0x240
};

struct UnkAutsniperTunable_l3d3a74 {
    u8 pad_0x0[0xfc];
    f32 field_0xfc;
    u8 pad_0x100[0x14];
    f32 field_0x114;
};

struct UnkAutsniperVec3_l3d3a74 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkAutsniperPair_l3d3a74 {
    u64 first;
    u64 second;
};

struct UnkAutsniperShoot_l3d3a74 {
    u8 pad_0x0[0x8];
    UnkAutsniperOwner_l3d3a74* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkAutsniperTunable_l3d3a74* tunable;  // 0x28
    u8 pad_0x30[0x20];
    s32 state;        // 0x50
    u32 field_0x54;   // 0x54
    u8 answer[0xc];   // 0x58
    UnkAutsniperVec3_l3d3a74 facing;  // 0x64
    UnkAutsniperVec3_l3d3a74 target;  // 0x70
    u8 pad_0x7c[0xc];
    f32 field_0x88;
    f32 field_0x8c;
    u64 field_0x90;
    UnkAutsniperPair_l3d3a74 field_0x98;
    f32 field_0xa8;
    UnkAutsniperVec3_l3d3a74 angles;  // 0xac
    u8 pad_0xb8[0x10];
    f32 field_0xc8;
    bool field_0xcc;
    u8 pad_0xcd[0x12b58 - 0xcd];
    bool on_wall;  // 0x12b58
};
static_assert(__builtin_offsetof(UnkAutsniperShoot_l3d3a74, on_wall) == 0x12b58, "layout");

void fn_71003d3a74(UnkAutsniperShoot_l3d3a74* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->state = 1;
    self->field_0x54 = 0;
    fn_710117364c(self->owner->model, reinterpret_cast<const u64*>(&g_unk_71016492a8),
                  reinterpret_cast<UnkGrappleAnswer_b50*>(self->answer));
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->transform));
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    self->facing.x = fx;
    self->facing.y = fy;
    self->facing.z = fz;
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    self->target.x = tx;
    self->target.y = ty;
    self->target.z = tz;
    self->field_0x88 = 0.0f;
    self->field_0xa8 = 0.0f;
    self->field_0xc8 = 1.0f;
    UnkAutsniperTunable_l3d3a74* tunable = self->tunable;
    self->field_0x98 = UnkAutsniperPair_l3d3a74();
    self->field_0x90 = 0;
    self->field_0xcc = false;
    f32 a = tunable->field_0x114;
    f32 b = tunable->field_0xfc;
    self->angles.x = 1.5707964f;
    self->angles.y = 1.5707964f;
    self->angles.z = 1.5707964f;
    self->field_0x8c = a > b ? b : a;
    if (UnkActorTarget* character = self->owner->character) {
        fn_7101178e08(character, 6, 0, 0, -1.0f);
        fn_710117662c(character, 0xb, 0, true, true, -1.0f);
        base::reflection::refl_EmitU32(character, 0x46cb3337bfed7326, 0x71b15e29dc542a11, 0, &g_unk_710164b958,
                                       &g_unk_7172769740);
    }
    UnkAutsniperAI_l3d3a74* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71003da2e0()))
        fn_71003daedc(ai, true);
    fn_71003d3cf8(reinterpret_cast<UnkAutsniperSend_b58*>(self));
    if (UnkAutsniperMovement_l3d3a74* movement = self->owner->movement) {
        if (fn_71010cc3a4(movement) == 1)
            self->on_wall = (movement->field_0xa0c != 0) ^ (movement->field_0x971 != 0);
    }
}
