// Unit 684 (tools/tu_map.py; hint CChozoRobotSoldierDisruptionFieldAttack): fires the disruption field. A
// shock wave description (fn_71009f35b0) is filled from the attack's tunables, the owner's position and
// facing, and the effect and damage the AI component's mode picks, then spawned for the owner
// (fn_71009f51f8) and destroyed. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CShockWaveComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;
using base::reflection::UnkReflObject;
using base::reflection::UnkTunable;

extern UnkTunable g_unk_71726cb3f8;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726cbd28;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726cbd30;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726cbd38;

// The disruption field's tunables (g_unk_71726cb3f8).
struct UnkFieldTunables_l4697e8 {
    u8 pad_0x0[0x2c];
    f32 radius;    // 0x2c
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
};

// The attack component's tunables (g_unk_7172716160): the damage of the field in each mode.
struct UnkAttackTunables_l4697e8 {
    u8 pad_0x0[0xdc];
    f32 damage;  // 0xdc
    u8 pad_0xe0[0x18];
    f32 boosted_damage;  // 0xf8
};

struct UnkChozoRobotSoldierAI_l4697e8 : UnkReflObject {
    u8 pad_0x8[0x27790 - 0x8];
    bool boosted;  // 0x27790
};

struct UnkDamage_l4697e8 {
    u8 pad_0x0[0xb8];
};

struct UnkPoint_l4697e8 {
    f32 x;
    f32 y;
    f32 z;
};

// The shock wave description fn_71009f35b0 builds and fn_71003cc474 destroys.
struct UnkShockWaveDesc_l4697e8 {
    u8 pad_0x0[0x8];
    UnkDamage_l4697e8 damage;  // 0x8
    base::global::UnkStringData* effect;  // 0xc0: a string id
    u8 pad_0xc8[0x10c - 0xc8];
    u32 kind;  // 0x10c
    u8 pad_0x110[0x4];
    UnkPoint_l4697e8 position;  // 0x114
    u8 pad_0x120[0x4];
    f32 radius;  // 0x124
    u8 pad_0x128[0x4];
    f32 angle;  // 0x12c
    f32 field_0x130;
    f32 field_0x134;
    u8 pad_0x138[0x188 - 0x138];
    UnkWeakHandle* owner;  // 0x188
    bool tinted;  // 0x190
    u8 pad_0x191[0x3];
    CVector4D color;  // 0x194
    bool field_0x1a4;
    u8 pad_0x1a5[0x3];
    f32 field_0x1a8;
    u8 pad_0x1ac[0x4];
    base::global::UnkStringData* name;  // 0x1b0: a string id
    u8 pad_0x1b8[0x210 - 0x1b8];

    UnkShockWaveDesc_l4697e8() { fn_71009f35b0(reinterpret_cast<char*>(this)); }
    ~UnkShockWaveDesc_l4697e8() { fn_71003cc474(reinterpret_cast<UnkShockWaveMember*>(this)); }

    // The string ids live in the description, which its own constructor and destructor manage.
    CStrId& Effect() { return reinterpret_cast<CStrId&>(effect); }
    CStrId& Name() { return reinterpret_cast<CStrId&>(name); }
};
static_assert(sizeof(UnkShockWaveDesc_l4697e8) == 0x210, "");
static_assert(__builtin_offsetof(UnkShockWaveDesc_l4697e8, name) == 0x1b0, "");

// Spawns the shock wave described for its owner.
void fn_71009f51f8(UnkShockWaveDesc_l4697e8* desc, UnkActor* owner);

// Picks the field's effect for the AI component's mode and returns the damage that goes with it.
static inline f32 SetEffect(UnkShockWaveDesc_l4697e8* desc, bool boosted) {
    const UnkAttackTunables_l4697e8& attack = reinterpret_cast<const UnkAttackTunables_l4697e8&>(g_unk_7172716160);
    if (boosted) {
        desc->Effect() = g_unk_71726cbd38;
        return attack.boosted_damage;
    }
    desc->Effect() = g_unk_71726cbd30;
    return attack.damage;
}

void fn_71004697e8(CAttack* self) {
    const UnkFieldTunables_l4697e8& field = reinterpret_cast<const UnkFieldTunables_l4697e8&>(g_unk_71726cb3f8);
    f32 radius = field.radius;
    f32 field_0x30 = field.field_0x30;
    f32 field_0x34 = field.field_0x34;
    f32 field_0x38 = field.field_0x38;
    UnkShockWaveDesc_l4697e8 desc;
    AssignRef(&desc.owner, GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(self->field_0x8)));
    desc.Name() = g_unk_71726cbd28;
    UnkActor* owner = self->field_0x8;
    UnkPoint_l4697e8 position = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    desc.radius = radius;
    desc.position = position;
    desc.field_0x130 = field_0x30;
    desc.field_0x134 = field_0x34;
    f32 facing = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38)).m[2];
    f32 angle = facing > -0.001f ? 1.5707964f : -1.5707964f;
    desc.kind = 1;
    desc.angle = angle;

    UnkReflObject* object = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    auto* ai = static_cast<UnkChozoRobotSoldierAI_l4697e8*>(
        fn_710009d50c(object->vf_0x0(), fn_710046ea2c()) ? object : nullptr);
    fn_71008c1538(reinterpret_cast<UnkRequest*>(&desc.damage), SetEffect(&desc, ai->boosted));
    desc.field_0x1a4 = true;
    desc.tinted = true;
    desc.field_0x1a8 = field_0x38;
    base::math::UnkVector4D color(0.0f, 1.0f, 1.0f, 0.5f);
    fn_710007b518(&desc.color, &color);
    fn_71009f51f8(&desc, self->field_0x8);
}
