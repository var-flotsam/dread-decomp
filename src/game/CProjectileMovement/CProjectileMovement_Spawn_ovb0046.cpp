// CProjectileMovement's unit (tools/tu_map.py, unit 1623). fn_7100ac38fc is the velocity update of
// src/game/CProjectileMovement/CProjectileMovement_Velocity.cpp (fn_7100ac64cc) under another address.
// fn_7100ac46f0 spawns the movement's effect (+0x490, a file-path id) through the model manager at a
// point raised by 20 with the given angles; fn_7100ac3efc runs the move (virtual slot 0x2b0) and, after a
// successful one unless the definition (+0x5c8) says otherwise, re-aims (slot 0x348, from fn_710007a7f0
// of +0x548) and hands the owner to fn_7100232c68. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CProjectileMovement.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU285.h"
#include "game/UnkTU341.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkProjectileState_ovb0046 {
    u8 unk_0x0[0x130];
    bool flag;
};

struct UnkProjectileMovementVel_ovb0046 {
    void** vtable;
    u8 unk_0x8[0x38];
    f32 dir_x;  // 0x40
    f32 dir_y;
    f32 dir_z;
    u8 unk_0x4c[0x30];
    f32 scale;  // 0x7c
    u8 unk_0x80[0x438];
    f32 vel_x;  // 0x4b8
    f32 vel_y;
    f32 vel_z;
    u8 unk_0x4c4[0xf4];
    f32 abs_y;  // 0x5b8
    f32 abs_x;
    f32 speed;  // 0x5c0
    u8 unk_0x5c4[0x4];
    UnkProjectileState_ovb0046* state;  // 0x5c8

    f32 vf_0x2f8() { return reinterpret_cast<f32 (*)(UnkProjectileMovementVel_ovb0046*)>(vtable[0x2f8 / 8])(this); }
};

void fn_7100ac38fc(UnkProjectileMovementVel_ovb0046* self) {
    f32 speed = self->vf_0x2f8() * self->scale;
    f32 x = self->dir_x;
    f32 y = self->dir_y;
    f32 z = self->dir_z;
    f32 vx = x * speed;
    f32 vy = speed * y;
    f32 vz = speed * z;
    self->vel_x = vx;
    self->vel_y = vy;
    self->vel_z = vz;
    if (self->state && self->state->flag) {
        self->speed = speed;
        self->abs_y = __builtin_fabsf(y);
        self->abs_x = __builtin_fabsf(x);
    }
}

// The effect object the model manager hands out, as its virtual calls see it (as in
// src/game/CMeleeComponent/CMeleeComponent_Checks_ovb0041.cpp).
struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0041 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};
struct UnkFxObject_ovb0041 {
    UnkFxObjectVtable_ovb0041* vtable;
};

struct UnkSpawnedFx_ovb0046 {
    u8 pad_0x0[0x108];
    f32 field_0x108;
    u8 pad_0x10c[0x4e];
    bool field_0x15a;
};

struct UnkProjectileFx_ovb0046 {
    u8 pad_0x0[0x490];
    base::UnkStrIdMemberOf<g_unk_7101d47200> effect;  // 0x490
};

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_7100ac46f0(UnkProjectileFx_ovb0046* self, const UnkVec3f* at, const UnkVec3f* angle) {
    if (self->effect.data == &g_unk_7101d47200->empty)
        return;
    UnkModelManager* manager = ModelManager();
    UnkVec3f position(at->x + 0.0f, at->y + 0.0f, at->z + 20.0f);
    UnkVec3f rotation = *angle;
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, &self->effect, false, true);
    if (!fx)
        return;
    fx->vtable->set_position(fx, &position);
    fx->vtable->set_rotation(fx, &rotation);
    fx->vtable->set_scale(fx, &scale);
    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
    fx->vtable->set_loop(fx, false);
    fx->vtable->set_speed(fx, 1.0f);
    fn_71011a9bb4(manager, fx);
    UnkSpawnedFx_ovb0046* spawned = reinterpret_cast<UnkSpawnedFx_ovb0046*>(fx);
    spawned->field_0x15a = false;
    spawned->field_0x108 = 1.0f;
    fn_71002093c8(reinterpret_cast<char*>(fx) + 0x28, false);
}

extern const u8 g_unk_710164b89c[] __attribute__((visibility("hidden")));

UnkVec3f fn_710007a7f0(UnkVec3f dir);  // direction to rotation angles (src/game_s1/fn_71001bc8a4.cpp)

// An intrusively counted result (count at +8, deleted through its slot 2).
struct UnkMoveResult_ovb0046 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkMoveResultRef_ovb0046 {
    UnkMoveResult_ovb0046* ptr = nullptr;
    void Release() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkMoveOwner_ovb0046 {
    u8 pad_0x0[0x1b5];
    bool frozen;  // 0x1b5
};

struct UnkMoveDef_ovb0046 {
    u8 pad_0x0[0x118];
    bool field_0x118;
};

struct UnkProjectileUpdate_ovb0046 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkMoveOwner_ovb0046* owner;  // 0x30
    u8 pad_0x38[0x4c];
    u8 field_0x84[0x4a4];
    u8 field_0x528[0x14];
    u8 field_0x53c[0xc];
    UnkVec3f field_0x548;
    u8 pad_0x554[0x74];
    UnkMoveDef_ovb0046* def;  // 0x5c8

    bool vf_0x2b0(void* a, void* arg, bool b, bool c, UnkMoveResultRef_ovb0046* out) {
        return reinterpret_cast<bool (*)(UnkProjectileUpdate_ovb0046*, void*, void*, bool, bool,
                                         UnkMoveResultRef_ovb0046*)>(vtable[0x2b0 / 8])(this, a, arg, b, c, out);
    }
    void vf_0x348(void* at, UnkVec3f* angle, const void* a, const void* b) {
        reinterpret_cast<void (*)(UnkProjectileUpdate_ovb0046*, void*, UnkVec3f*, const void*, const void*)>(
            vtable[0x348 / 8])(this, at, angle, a, b);
    }
};
static_assert(__builtin_offsetof(UnkProjectileUpdate_ovb0046, def) == 0x5c8, "layout");

bool fn_7100ac3ff4(UnkProjectileUpdate_ovb0046* self, void* target);

static inline UnkVec3f* Ptr(const UnkVec3f& v) { return const_cast<UnkVec3f*>(&v); }

bool fn_7100ac3efc(UnkProjectileUpdate_ovb0046* self, void* arg) {
    if (self->owner->frozen)
        return false;
    UnkMoveResultRef_ovb0046 result;
    bool moved = self->vf_0x2b0(self->field_0x84, arg, false, true, &result);
    result.Release();
    if (!moved)
        return false;
    if (!self->def->field_0x118) {
        if (!fn_7100ac3ff4(self, self->field_0x528))
            return true;
        self->vf_0x348(self->field_0x53c, Ptr(((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(self->field_0x548)),
                       g_unk_710164b89c, &g_unk_7101d470e8);
    }
    fn_7100232c68(self->owner);
    return true;
}
