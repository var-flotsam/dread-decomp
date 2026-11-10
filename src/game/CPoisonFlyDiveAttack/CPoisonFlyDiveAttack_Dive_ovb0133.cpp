// The CPoisonFlyDiveAttack unit (tools/tu_map.py). Names and layouts are placeholders.
// fn_710063208c, the attack's start: after the base (fn_71003608ac) the direction (+0x48) is cleared,
//   state 1 (+0x54) set with the flag at +0x58, the word at +0x5c cleared and the owner's (+0x8) AI
//   (+0x240, as a CPoisonFlyAIComponent) given value 1 (fn_710111094c); then the side (+0x60) and the
//   direction become +x or -x, towards the actor the owner acts against (fn_7100d7c990), and the start
//   (+0x30) is the owner's position.
// fn_71006312bc, whether the dive may start: the base check (fn_7100360b84), an actor to act against not
//   far above the owner (the unit direction's y at most 0.1), the AI manager's registry
//   (g_unk_71726af468, created on first use, +0x80) accepting the owner's position (fn_710063a7ec) and
//   the owner (fn_710063c2cc) and putting it farther away than its body reach (fn_7100977b20 of +0x1f8;
//   fn_710063c10c gives the squared distance). Without the position accepted, the
//   CPoisonFlyAIComponent's counter at +0x14770 is cleared when positive.
// fn_710063302c: in state 2, once the AI agrees (vf_0x448) and the registry puts the owner farther away
//   than 1.5 times its body reach the dive starts (fn_71006314b4) in state 3, otherwise it gives up
//   (state 6, fn_7100632204); state 3 moves to state 4 as fn_7100632204 does (a kind-5 block carrying
//   the vector at +0x48 for the owner's +0x298).
#include "base/UnkVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern "C" float sqrtf(float);

f32 fn_7100977b20(void*);
f32 fn_710063c10c(void* registry, UnkActor* actor);

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_710111094c(UnkReflObject* ai, u32 value);

struct UnkV3s_ovb0133 {
    f32 x, y, z;
};

static inline UnkV3s_ovb0133 operator-(const UnkV3s_ovb0133& a, const UnkV3s_ovb0133& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline UnkV3s_ovb0133 Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

struct UnkV3w_ovb0133 {
    u32 x, y, z;
};

struct UnkPoisonFlyDive3_ovb0133 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkV3w_ovb0133 start;  // 0x30
    u8 pad_0x3c[0xc];
    UnkVec3f dir;  // 0x48
    u32 state;  // 0x54
    bool field_0x58;
    u8 pad_0x59[3];
    u32 field_0x5c;
    UnkVec3f side;  // 0x60
};

void fn_710063208c(UnkPoisonFlyDive3_ovb0133* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->dir = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
    self->field_0x58 = true;
    self->state = 1;
    self->field_0x5c = 0;
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    fn_710111094c(ai && fn_710009d50c(ai->vf_0x0(), fn_7100638be0()) ? ai : nullptr, 1);
    UnkActor* other = fn_7100d7c990(self->owner);
    UnkV3s_ovb0133 d = Pos(other) - Pos(self->owner);
    f32 inv = 1.0f / (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
    f32 x = d.x * inv, y = d.y * inv, z = d.z * inv;
    f32 dot = x * 1.0f + y * 0.0f + z * 0.0f;
    UnkVec3f v = dot > 0.0f ? UnkVec3f(1.0f, 0.0f, 0.0f) : UnkVec3f(-1.0f, -0.0f, -0.0f);
    self->side = v;
    self->dir = v;
    UnkActor* owner = self->owner;
    u32 px = *reinterpret_cast<u32*>(&owner->field_0x44);
    u32 py = *reinterpret_cast<u32*>(&owner->field_0x54);
    u32 pz = *reinterpret_cast<u32*>(&owner->field_0x64);
    self->start.x = px;
    self->start.y = py;
    self->start.z = pz;
}

struct UnkPosWords_ovb0133 {
    u32 x, y, z;
};

bool fn_710063a7ec(void* registry, UnkActor* actor, const UnkPosWords_ovb0133* pos);
bool fn_710063c2cc(void* registry, UnkActor* actor);

struct UnkPoisonFlyAICounter_ovb0133 {
    u8 pad_0x0[0x14770];
    s32 counter;  // 0x14770
};

struct UnkPoisonFlyDive6_ovb0133 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

bool fn_71006312bc(UnkPoisonFlyDive6_ovb0133* self, void* arg, u32 mode) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode);
    UnkActor* other = fn_7100d7c990(self->owner);
    if (!other)
        return false;
    UnkV3s_ovb0133 d = Pos(other) - Pos(self->owner);
    f32 inv = 1.0f / (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
    ok = ok & (d.y * inv <= 0.1f);
    f32 reach = fn_7100977b20(self->owner->field_0x1f8);
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    void* registry = reinterpret_cast<char*>(m) + 0x80;
    if (ok) {
        UnkActor* owner = self->owner;
        u32 x = *reinterpret_cast<const u32*>(&owner->field_0x44);
        u32 y = *reinterpret_cast<const u32*>(&owner->field_0x54);
        u32 z = *reinterpret_cast<const u32*>(&owner->field_0x64);
        UnkPosWords_ovb0133 pos = {x, y, z};
        ok = fn_710063a7ec(registry, owner, &pos);
    }
    if (UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240)) {
        auto* cast = fn_710009d50c(ai->vf_0x0(), fn_7100638be0()) ? reinterpret_cast<UnkPoisonFlyAICounter_ovb0133*>(ai)
                                                                  : nullptr;
        if (!(ok | (cast == nullptr))) {
            if (cast->counter >= 1)
                cast->counter = 0;
        }
    }
    if (ok && fn_710063c2cc(registry, self->owner))
        return fn_710063c10c(registry, self->owner) > reach * reach;
    return false;
}


struct UnkPoisonFlyDive_b61;
void fn_7100632204(UnkPoisonFlyDive_b61* self, u32 state);

// The kind-5 block (game_s2/UnkParams.h) with its first fields seen as words.
struct UnkPoisonFlyParams_ovb0133 {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkPoisonFlyDive_ovb0133 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x48 - 0x10];
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 state;  // 0x54
};

void fn_71006314b4(UnkPoisonFlyDive_ovb0133* self);

static inline bool AiAgrees(void* ai) {
    u64 b = 0;
    u64 a = 0;
    using Fn = bool (*)(void*, u32, u64*, u64*, u32, u32, void*, void*);
    return (*reinterpret_cast<Fn**>(ai))[0x448 / 8](ai, 1, &b, &a, 3, 1, nullptr, nullptr);
}

// State 4, as fn_7100632204 sets it.
static inline void EnterState4(UnkPoisonFlyDive_ovb0133* self) {
    self->state = 4;
    void* target = self->owner->field_0x298;
    UnkPoisonFlyParams_ovb0133 params;
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    params.field_0x8 = self->field_0x48;
    params.field_0xc = self->field_0x4c;
    params.field_0x10 = self->field_0x50;
    params.field_0x14 = 1;
    fn_71006f4b0c(target, &params, 1);
}

void fn_710063302c(UnkPoisonFlyDive_ovb0133* self) {
    switch (self->state) {
    case 2: {
        UnkManager_71726af468* m = g_unk_71726af468;
        if (!m) {
            m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
            fn_7100da7e2c(m);
            g_unk_71726af468 = m;
        }
        f32 reach = fn_7100977b20(self->owner->field_0x1f8);
        if (AiAgrees(self->owner->field_0x240)) {
            f32 r = reach * 1.5f;
            if (fn_710063c10c(reinterpret_cast<char*>(m) + 0x80, self->owner) > r * r) {
                fn_71006314b4(self);
                self->state = 3;
                return;
            }
        }
        fn_7100632204(reinterpret_cast<UnkPoisonFlyDive_b61*>(self), 6);
        break;
    }
    case 3:
        EnterState4(self);
        break;
    }
}
