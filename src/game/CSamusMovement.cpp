// CSamusMovement's unit (tools/tu_map.py unit 1501: 0x71009dc3bc-0x71009f16a8).
// Holds fn_71009e0160: the per-frame update of the legs' override state. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The two leg nodes (+0x5468, +0x5470): vf_0x80 resets one, the key at +0x20 of its +0x10 names
// its node in the shape (fn_71011735e8), and fn_71002d60c4 sets its weight.
struct UnkLegNodeInfo {
    u8 pad_0x0[0x20];
    u64 key;
};
struct UnkLegNode {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18();
    virtual void vf_0x20(); virtual void vf_0x28(); virtual void vf_0x30(); virtual void vf_0x38();
    virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78();
    virtual void vf_0x80();
    u8 pad_0x8[0x8];
    UnkLegNodeInfo* field_0x10;
};
void fn_71002d60c4(UnkLegNode* self, f32 weight);

// The current mode (+0x2d08): its kind at +0x10 (0xf when there is none).
struct UnkSamusMode {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkSamusMovement {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0xf0];
    UnkWeakHandle* field_0x128;  // a target
    u8 pad_0x130[0x24];
    f32 field_0x154[3];  // a direction
    u8 pad_0x160[0x248];
    bool field_0x3a8;
    u8 pad_0x3a9[0x295f];
    UnkSamusMode* mode;
    u8 pad_0x2d10[0x448];
    f32 field_0x3158;
    u8 pad_0x315c[0x40c];
    f32 field_0x3568;
    u8 pad_0x356c[0x1cf4];
    u8 timer[0xb0];  // +0x5260 (fn_710002fa90 restarts it, fn_710002fb70 tests it)
    u32 field_0x5310;
    u8 pad_0x5314[0x10];
    u32 field_0x5324;
    u32 field_0x5328;
    UnkVec3f field_0x532c;  // the two leg positions (reset to g_unk_710164779c)
    UnkVec3f field_0x5338;
    u8 pad_0x5344[0x124];
    UnkLegNode* legs[2];
};
static_assert(__builtin_offsetof(UnkSamusMovement, mode) == 0x2d08, "UnkSamusMovement placeholder layout");
static_assert(__builtin_offsetof(UnkSamusMovement, field_0x3158) == 0x3158, "UnkSamusMovement placeholder layout");
static_assert(__builtin_offsetof(UnkSamusMovement, field_0x3568) == 0x3568, "UnkSamusMovement placeholder layout");
static_assert(__builtin_offsetof(UnkSamusMovement, timer) == 0x5260, "UnkSamusMovement placeholder layout");
static_assert(__builtin_offsetof(UnkSamusMovement, field_0x532c) == 0x532c, "UnkSamusMovement placeholder layout");
static_assert(__builtin_offsetof(UnkSamusMovement, legs) == 0x5468, "UnkSamusMovement placeholder layout");

// A settings object (reached through the GOT): the legs' override is off while its +0x78 is clear.
struct UnkSamusSettings {
    u8 pad_0x0[0x78];
    bool field_0x78;
};
extern UnkSamusSettings g_unk_7172740de0;

// The two leg keys fn_71009eee34 is given.
struct UnkLegKeys {
    u64 field_0x0;
    u64 field_0x8;
};
extern const UnkLegKeys g_unk_7172741018 __attribute__((visibility("hidden")));

struct UnkEntity;
struct UnkFlagged;
void fn_710002fa90(void* timer);
bool fn_710002fb70(void* timer);
bool fn_71011735e8(UnkEntity* entity, const void* key, UnkMatrix44* out, u32 flags);
f32 fn_7100977bec(UnkFlagged* flagged);
void fn_71009eee34(UnkSamusMovement* self, const u64* key, bool active, bool flag);
void fn_71009ef110(UnkSamusMovement* self, const UnkVec3f* a, const UnkVec3f* b);
void fn_71009eff9c(UnkSamusMovement* self, bool flag, f32 dt);

static inline bool HasTarget(const UnkSamusMovement* self) {
    return self->field_0x128 && self->field_0x128->target;
}

static inline UnkAnimationComponent* Anim(const UnkSamusMovement* self) {
    return static_cast<UnkAnimationComponent*>(self->actor->field_0x220);
}

static inline f32 Dot(UnkVec3f a, UnkVec3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline void Identity(UnkMatrix44* m) {
    m->m[0] = 1.0f;
    m->m[1] = 0.0f;
    m->m[2] = 0.0f;
    m->m[3] = 0.0f;
    m->m[4] = 0.0f;
    m->m[5] = 1.0f;
    m->m[6] = 0.0f;
    m->m[7] = 0.0f;
    m->m[8] = 0.0f;
    m->m[9] = 0.0f;
    m->m[10] = 1.0f;
    m->m[11] = 0.0f;
    m->m[12] = 0.0f;
    m->m[13] = 0.0f;
    m->m[14] = 0.0f;
    m->m[15] = 1.0f;
}

// The translation column of a matrix.
static inline UnkVec3f Translation(const UnkMatrix44& m) {
    return UnkVec3f(m.m[3], m.m[7], m.m[11]);
}

// The legs' override for this frame. With the settings flag clear the timer at +0x5260 is restarted
// and both leg nodes reset. Otherwise the mode (for mode 2 also whether the actor faces away from
// the direction at +0x154) decides the state at +0x5310 and whether the override applies; the two
// leg nodes are told so (fn_71009eee34). Unless fn_710002fb70 holds for the timer the two leg
// positions are reset; when it does (and no deactivating track plays) they follow the two nodes'
// positions in the shape at the actor's height, with both nodes at full weight after a slope-IK track
// or when `flag` is set. Then the movement goes on (fn_71009eff9c).
// NON_MATCHING(minor): frame layout and scheduling: the original's frame is 0x10 bytes larger (the
// two leg points 16 bytes apart; x28 saved with x25), its identity matrices' constant stores are
// paired differently, and it keeps the leg points' x and z in registers across fn_7100977bec where
// ours stores them before the call; the same calls, loads, compares and stored values.
void fn_71009e0160(UnkSamusMovement* self, bool flag, f32 dt) {
    if (!g_unk_7172740de0.field_0x78) {
        self->field_0x5324 = 0;
        fn_710002fa90(self->timer);
        self->legs[0]->vf_0x80();
        self->legs[1]->vf_0x80();
        return;
    }
    bool facing = false;
    if (HasTarget(self)) {
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
        f32 d = Dot(UnkVec3f(m.m[2], m.m[6], m.m[10]),
                    UnkVec3f(self->field_0x154[0], self->field_0x154[1], self->field_0x154[2]));
        facing = (!(d > 0.1f)) & (d < -0.1f);
    }
    s32 mode = self->mode ? self->mode->kind : 0xf;
    bool valid = false;
    bool value = false;
    if (!(fn_710117b348(Anim(self), fn_7100d25ea8()) && HasTarget(self))) {
        switch (mode) {
        case 8:
            if (self->field_0x3a8) {
                valid = true;
                if (UnkAnimationComponent* anim = Anim(self))
                    value = fn_710117b348(anim, fn_7100cf3a18());
            }
            break;
        case 2:
            valid = true;
            if (facing && Anim(self) && !fn_710117ae38(Anim(self), 0x93c0fb74bf1f5e37, 0) &&
                !fn_710117ae38(Anim(self), 0x62197662a106179f, 0))
                value = self->field_0x3158 >= 0.0f && self->field_0x3158 <= 0.04f;
            break;
        case 3:
        case 6:
        case 7:
            break;
        case 4:
            valid = true;
            if (self->field_0x3568 > 0.04f)
                value = true;
            break;
        case 5:
            valid = self->field_0x3a8;
            break;
        }
    }
    if (self->field_0x5310 != value)
        self->field_0x5310 = value;
    bool ignore_slope = fn_710117b348(Anim(self), fn_7100c34a78());
    fn_71009eee34(self, &g_unk_7172741018.field_0x0, valid, ignore_slope);
    fn_71009eee34(self, &g_unk_7172741018.field_0x8, fn_710117b348(Anim(self), fn_7100c48a08()), ignore_slope);
    if (!fn_710002fb70(self->timer)) {
        // The constant is a CVector3D; the fields are the same layout, copied word by word (UnkVec3f).
        const UnkVec3f& zero = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
        self->field_0x532c = zero;
        self->field_0x5338 = zero;
    }
    self->field_0x5328 = 0;
    if (fn_710002fb70(self->timer) && !fn_710117b348(Anim(self), fn_7100c48f18())) {
        UnkEntity* shape = static_cast<UnkEntity*>(self->actor->field_0x228);
        UnkMatrix44 a;
        Identity(&a);
        {
            u64 key = self->legs[0]->field_0x10->key;
            fn_71011735e8(shape, &key, &a, 0);
        }
        UnkMatrix44 b;
        Identity(&b);
        {
            u64 key = self->legs[1]->field_0x10->key;
            fn_71011735e8(shape, &key, &b, 0);
        }
        UnkVec3f pa = Translation(a);
        UnkVec3f pb = Translation(b);
        f32 height = fn_7100977bec(static_cast<UnkFlagged*>(self->actor->field_0x208));
        f32 y = self->actor->field_0x54 - height;
        pa.y = y;
        pb.y = y;
        fn_71009ef110(self, &pa, &pb);
        if (ignore_slope || flag) {
            fn_71002d60c4(self->legs[1], 1.0f);
            fn_71002d60c4(self->legs[0], 1.0f);
        }
    }
    fn_71009eff9c(self, flag, dt);
}
