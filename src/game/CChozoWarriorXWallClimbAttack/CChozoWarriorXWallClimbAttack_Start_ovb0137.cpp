// CChozoWarriorXWallClimbAttack's unit (tools/tu_map.py): the attack's start, with a new trail effect (as src/game/CChozoWarriorXSpitMovementComponent/CChozoWarriorXSpitMovementComponent_Launch_ovb0127.cpp creates one).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU252.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
struct UnkEffectSink;
void fn_71003608ac(UnkAttackBase* self);
base::global::CStrId fn_710049c2f4(UnkReflObject* ai, f32 facing);
void* fn_7100149930(UnkRenderPool*, void*, UnkSceneObject**, base::global::CStrId*);

// A colour (its copy constructor makes it passed through memory).
struct UnkTrailColor_ovb0137 {
    f32 r, g, b, a;
    UnkTrailColor_ovb0137(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
    UnkTrailColor_ovb0137(const UnkTrailColor_ovb0137& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
};
void fn_7100148190(UnkSceneObject* self, u32 kind, UnkTrailColor_ovb0137 color, bool flag, f32 a, f32 b, f32 c);

struct UnkWallClimbParams_ovb0137 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkWallClimbAttack_ovb0137 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x38];
    UnkSceneObject* trail;  // 0x48
};

// The attack's start: after the base start, a kind-3 parameter block for the owner's effect sink and
// the AI's pick for the negated facing x (as fn_71004954d4), then a new trail effect at +0x48.
void fn_710048cbe8(UnkWallClimbAttack_ovb0137* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkReflObject* ai = *reinterpret_cast<UnkReflObject**>(self->owner + 0x240);
    if (ai) {
        UnkEffectSink* sink = *reinterpret_cast<UnkEffectSink**>(self->owner + 0x218);
        if (fn_710009d50c(ai->vf_0x0(), fn_710048db44()) && sink) {
            UnkWallClimbParams_ovb0137 params;
            params.field_0xc = 0;
            params.field_0x14 = 0;
            __builtin_memset(params.field_0x18, 0, 13);
            __builtin_memset(params.field_0x28, 0, 13);
            params.vtable = g_unk_7101a2e208 + 2;
            params.field_0x3d = 0;
            params.field_0x38 = 3;
            fn_71010c7e6c(sink, &params, 1);
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner + 0x38));
            fn_710049c2f4(ai, -m.m[2]);
        }
    }
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkSceneObject* trail = nullptr;
    fn_7100149930(manager->field_0x218, nullptr, &trail, &g_unk_7101d470e0);
    fn_7100219ae0(manager, trail ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(trail) + 8) : nullptr, 0);
    self->trail = trail;
    fn_7100148190(trail, 6, UnkTrailColor_ovb0137(1.0f, 0.0f, 0.0f, 1.0f), true, 425.0f, 50.0f, 1.0f);
    fn_71001beee4(self->trail->field_0x130, 1);
    fn_7100149490(self->trail, false);
}
