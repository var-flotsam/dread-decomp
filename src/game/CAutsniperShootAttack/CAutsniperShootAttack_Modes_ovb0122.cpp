// CAutsniperShootAttack's unit (tools/tu_map.py unit 576): the attack's per-frame update and its mode
// switches. The attack keeps a state (+0x50) and an aiming mode (+0x54, 2 while the shot is locked, when
// the animation key 0xf4061ff25a2163d6 is set on the owner's animation component and the aim sound is
// stopped). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

using base::reflection::UnkReflObject;

// As src/game/CHeatableShieldComponent/CHeatableShieldEntry_Reset_A5_b71.cpp spells the handle.
struct UnkSound;
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};

// As src/game/CEntity_2.cpp, src/game/CShipRechargeComponent/CShipRechargeComponent_StopSound_b49a4.cpp,
// src/game/CAutsniperShootAttack/fn_71003d5a14.cpp and src/game/CAreaFXComponent/CAreaFXComponent_b41.cpp
// declare them.
struct UnkEntityWithSound;
struct UnkShipRechargeEntity_b49a4;
UnkSoundHandle fn_7100d7c590(UnkEntityWithSound* self, void* a);
void fn_7100d7c510(UnkShipRechargeEntity_b49a4* entity, UnkSoundHandle* handle, f32 fade, f32 delay);
void fn_710117abfc(void* target, u32 a1, void* a2, u64 id);
struct UnkSelf;
struct UnkVec3;
void fn_71003d5450(UnkSelf* self, const UnkVec3* v, u32 a2);
u32 fn_71010cc3a4(void*);
extern const base::math::CVector2D g_unk_7101647780;

bool fn_71010cab0c(void* movement);

// The aim sound's id (+0x0) and the shot's animation id (+0x18).
struct UnkAutsniperIds_ovb0122 {
    u8 sound[0x18];
    u8 anim[0x8];
};
extern UnkAutsniperIds_ovb0122 g_unk_71726bf838 __attribute__((visibility("hidden")));
extern char g_unk_71726bf850[] __attribute__((visibility("hidden")));

struct UnkAutsniperVec3_ovb0122 {
    u32 x, y, z;
};

struct UnkAutsniperMovement_ovb0122 {
    u8 pad_0x0[0x971];
    u8 field_0x971;
    u8 pad_0x972[0x9a];
    u8 field_0xa0c;
};
static_assert(__builtin_offsetof(UnkAutsniperMovement_ovb0122, field_0xa0c) == 0xa0c, "layout");

struct UnkAutsniperOwner_ovb0122 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x178];
    UnkReflObject* movement;  // 0x1f0, a CEnemyMovement
    u8 pad_0x1f8[0x20];
    UnkAutsniperMovement_ovb0122* field_0x218;
    void* anim;  // 0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // 0x240, a CAutsniperAIComponent
};
static_assert(__builtin_offsetof(UnkAutsniperOwner_ovb0122, ai) == 0x240, "layout");

struct UnkAutsniperParams_ovb0122 {
    u8 pad_0x0[0xfc];
    f32 field_0xfc;
    u8 pad_0x100[0x24];
    f32 wait_time;    // 0x124
    f32 field_0x128;  // 0x128
};

struct UnkAutsniperShootAttack_ovb0122 {
    void** vtable;
    UnkAutsniperOwner_ovb0122* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkAutsniperParams_ovb0122* params;  // 0x28
    u8 pad_0x30[0x20];
    u32 state;  // 0x50
    u32 mode;   // 0x54
    u8 field_0x58[0xc];
    f32 position[3];  // 0x64
    u8 pad_0x70[0xc];
    UnkAutsniperVec3_ovb0122 target;  // 0x7c
    u8 pad_0x88[0x1c];
    f32 wait_timer;    // 0xa4
    f32 active_timer;  // 0xa8
    u8 pad_0xac[0x20];
    bool active;  // 0xcc
};
static_assert(__builtin_offsetof(UnkAutsniperShootAttack_ovb0122, active) == 0xcc, "layout");

void fn_71003d5aac(UnkAutsniperShootAttack_ovb0122* self, u32 mode);
void fn_71003d49f0(UnkAutsniperShootAttack_ovb0122* self, u8* out);
void fn_71003d4864(UnkAutsniperShootAttack_ovb0122* self, u8* out);
void fn_71003d415c(UnkAutsniperShootAttack_ovb0122* self);
void fn_71003d3f20(UnkAutsniperShootAttack_ovb0122* self, f32 dt);
void fn_71003d7060(UnkAutsniperShootAttack_ovb0122* self);

static inline bool CallVf448_ovb0122(UnkReflObject* ai, void* out) {
    using Fn = bool (*)(UnkReflObject*, u32, const base::math::CVector2D*, const base::math::CVector2D*, u32, u32,
                        void*, void*);
    return (*reinterpret_cast<Fn**>(ai))[0x448 / 8](ai, 1, &g_unk_7101647780, &g_unk_7101647780, 3, 7, nullptr, out);
}

static inline void CallVf668_ovb0122(UnkReflObject* ai) {
    using Fn = void (*)(UnkReflObject*);
    (*reinterpret_cast<Fn**>(ai))[0x668 / 8](ai);
}

// Locks the shot (mode 2) while the owner's movement is a CEnemyMovement in state 4 and the attack is
// in state 4; otherwise switches to mode 1 (fn_71003d5aac).
void fn_71003d57dc(UnkAutsniperShootAttack_ovb0122* self) {
    UnkReflObject* movement = self->owner->movement;
    if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) || fn_71010cc3a4(movement) != 4) {
        fn_71003d5aac(self, 1);
        return;
    }
    if (self->state == 4 && __builtin_expect(self->mode != 2, 1)) {
        self->mode = 2;
        fn_710117abfc(self->owner->anim, 1, g_unk_71726bf838.anim, 0xf4061ff25a2163d6);
        UnkSoundHandle h = fn_7100d7c590(reinterpret_cast<UnkEntityWithSound*>(self->owner), &g_unk_71726bf838);
        if (h.ref && h.serial != -1 && h.ref->serial == h.serial && h.ref->sound)
            fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(self->owner), &h, 0.0f, 0.0f);
    }
}

// Releases a pending mode once the AI's check (vf_0x448) and the two range tests (fn_71003d49f0,
// fn_71003d4864) allow it and the state is not 4 to 6: mode 2 clears the animation key, any other mode
// aims at the target (+0x7c).
void fn_71003d58f8(UnkAutsniperShootAttack_ovb0122* self) {
    bool hit = CallVf448_ovb0122(self->owner->ai, self->field_0x58);
    u8 a = 1;
    fn_71003d49f0(self, &a);
    u8 b = 1;
    fn_71003d4864(self, &b);
    if (!b || (hit & (a == 0)))
        return;
    if (self->state - 4 < 3)
        return;
    u32 mode = self->mode;
    if (!mode)
        return;
    self->mode = 0;
    if (mode == 2) {
        fn_710117abfc(self->owner->anim, 0, g_unk_71726bf850, 0xf4061ff25a2163d6);
        return;
    }
    UnkAutsniperVec3_ovb0122 v;
    v.x = self->target.x;
    v.y = self->target.y;
    v.z = self->target.z;
    fn_71003d5450(reinterpret_cast<UnkSelf*>(self), reinterpret_cast<const UnkVec3*>(&v), 0);
}

// The two timers: while inactive the wait timer runs up to the wait time, then the AI is told
// (vf_0x668, its +0x179e) and the attack turns active; while active the other timer runs up to
// +0x128 - +0xfc, then the attack enters state 4, flags the CAutsniperAIComponent (+0x27248) and goes
// on with fn_71003d7060.
void fn_71003d6894(UnkAutsniperShootAttack_ovb0122* self, f32 dt) {
    if (self->active) {
        self->active_timer += dt;
    } else {
        self->wait_timer += dt;
        if (self->wait_timer >= self->params->wait_time) {
            self->wait_timer = 0.0f;
            self->active = true;
            CallVf668_ovb0122(self->owner->ai);
            reinterpret_cast<u8*>(self->owner->ai)[0x179e] = 1;
        }
    }
    if (self->active && self->active_timer >= self->params->field_0x128 - self->params->field_0xfc) {
        self->active_timer = 0.0f;
        self->active = false;
        if (self->state == 4)
            return;
        self->state = 4;
        UnkReflObject* ai = self->owner->ai;
        u8* found = fn_710009d50c(ai->vf_0x0(), fn_71003da2e0()) ? reinterpret_cast<u8*>(ai) : nullptr;
        found[0x27248] = 1;
        fn_71003d7060(self);
    }
}

static inline void UpdateMode_ovb0122(UnkAutsniperShootAttack_ovb0122* self, f32 dt) {
    if (self->state != 7) {
        switch (self->mode) {
        case 0:
            fn_71003d415c(self);
            break;
        case 1:
            fn_71003d58f8(self);
            break;
        case 2:
            fn_71003d58f8(self);
            break;
        }
    }
    fn_71003d3f20(self, dt);
}

// The update: the base attack's (fn_71003609a8), the owner's facing column kept at +0x64 unless the
// movement (+0x218) says otherwise (fn_71010cab0c), the mode's step and fn_71003d3f20; with the
// movement in state 1, whether exactly one of its two flags is set goes to +0x12b58.
void fn_71003d3df0(UnkAutsniperShootAttack_ovb0122* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    UnkAutsniperMovement_ovb0122* movement = self->owner->field_0x218;
    if (!movement) {
        UpdateMode_ovb0122(self, dt);
        return;
    }
    if (!fn_71010cab0c(movement)) {
        const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
        f32 x = m.m[2];
        f32 y = m.m[6];
        f32 z = m.m[10];
        self->position[0] = x;
        self->position[1] = y;
        self->position[2] = z;
    }
    UpdateMode_ovb0122(self, dt);
    if (fn_71010cc3a4(movement) == 1)
        reinterpret_cast<u8*>(self)[0x12b58] = (movement->field_0xa0c != 0) ^ (movement->field_0x971 != 0);
}
