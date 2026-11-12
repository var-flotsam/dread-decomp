// CInfesterShootAttack's unit (tools/tu_map.py): the attack's start: after the base (fn_71003608ac) sets +0x30
// to 1, keeps the owner's (+0x8) character class (+0x220) at +0x58, stops its layers 0xb and 0xc and plays
// layer 6, hooks the callback fn_71005c7824 (link at +0x38, handle at +0x50) up to it, as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp, then hands the owner's navigation object
// (+0x298) a kind-5 parameter block (game_s2/UnkParams.h, fn_71006f4b0c with 1) facing along the owner's
// CEnemyMovement (+0x1f0) vf_0x210 direction, negated when fn_71005c6458 says so. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_71005c7824();
struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, bool, bool, f32);
void fn_710117662c(UnkActorTarget*, u32, u32, u32, u32, f32);

// Returned in memory (not trivially copyable).
struct UnkInfesterVec3_A3_b73 {
    f32 x, y, z;
    UnkInfesterVec3_A3_b73();
    UnkInfesterVec3_A3_b73(const UnkInfesterVec3_A3_b73& other);
};

struct UnkInfesterMovement_A3_b73 : UnkReflObject {
    UnkInfesterVec3_A3_b73 vf_0x210() {
        return reinterpret_cast<UnkInfesterVec3_A3_b73 (*)(UnkInfesterMovement_A3_b73*)>(
            (*reinterpret_cast<void***>(this))[0x210 / 8])(this);
    }
};

// The kind-5 block (game_s2/UnkParams.h) with its direction seen as floats.
struct UnkInfesterParams_A3_b73 {
    void** vtable;
    f32 x;
    f32 y;
    f32 z;
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

struct UnkInfesterTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkInfesterShoot_A3_b73 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u8 pad_0x34[0x4];
    UnkInfesterTrackLink_A3_b73 link;  // 0x38
    UnkWeakHandle* handle;             // 0x50
    UnkActorTarget* anim;              // 0x58
};

bool fn_71005c6458(UnkInfesterShoot_A3_b73* self);

void fn_71005c6914(UnkInfesterShoot_A3_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 1;
    self->anim = *reinterpret_cast<UnkActorTarget**>(self->owner + 0x220);
    fn_7101178e08(self->anim, 0xb, 0, 0, -1.0f);
    fn_7101178e08(self->anim, 0xc, 0, 0, -1.0f);
    fn_710117662c(self->anim, 6, 0, 0, 0, -1.0f);
    if (*reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220)) {
        UnkInfesterTrackLink_A3_b73* link = &self->link;
        link->context = self;
        link->callback = fn_71005c7824;
        UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        if (track) {
            asm("" : "+r"(self));
            fn_7100316018(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
    void* nav = *reinterpret_cast<void**>(self->owner + 0x298);
    UnkReflObject* m = *reinterpret_cast<UnkReflObject**>(self->owner + 0x1f0);
    UnkInfesterMovement_A3_b73* movement =
        m && fn_710009d50c(m->vf_0x0(), fn_71010c74e0()) ? static_cast<UnkInfesterMovement_A3_b73*>(m) : nullptr;
    UnkInfesterParams_A3_b73 params;
    params.field_0x30 = 0;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    __builtin_memset(&params.x, 0, 13);
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    if (fn_71005c6458(self)) {
        UnkInfesterVec3_A3_b73 v = movement->vf_0x210();
        params.x = -v.x;
        params.y = -v.y;
        params.z = -v.z;
    } else {
        UnkInfesterVec3_A3_b73 v = movement->vf_0x210();
        params.x = v.x;
        params.y = v.y;
        params.z = v.z;
    }
    params.field_0x14 = 1;
    fn_71006f4b0c(nav, &params, 1);
}
