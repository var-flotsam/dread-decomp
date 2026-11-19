// CBigkranXSpitAttack's unit (tools/tu_map.py unit 599): the attack's start. After the base start, hooks the
// callback fn_71003f4200 (link at +0x38, handle at +0x50) up to the owner's animation, turns the owner's
// navigation toward the side it faces (mirrored when the attack's track turns), picks a random entry of the
// pattern table, resets the timers, the target position and the aim, and aims (fn_71003f18f8) along the
// owner's angles. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/UnkMatrix44.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkSpitAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkAttackBase;
void fn_71003f4200();

// As src/game/CBigkranXSpitAttack/CBigkranXSpitAttack_Aim_ovb0267.cpp defines them.
struct UnkSpitAim_ovb0267;
struct UnkSpitVec3_ovb0267;
void fn_71003f18f8(UnkSpitAim_ovb0267* self, bool a, bool b, const UnkSpitVec3_ovb0267* dir);

// As src/game/nn/time/time_HasTrack_l06ba58.cpp defines it.
bool fn_710006ba58(void* tracks, const base::reflection::CClass* cls);

// The navigation's facing for an owner looking forward and backward.
extern const UnkVec3f g_unk_7101649374 __attribute__((visibility("hidden")));
extern const UnkVec3f g_unk_7101649380 __attribute__((visibility("hidden")));

struct UnkSpitStaticZero_l3f1594 {
    u32 value;
    UnkSpitStaticZero_l3f1594() : value(0) {}
};
inline const UnkSpitStaticZero_l3f1594& SpitStaticZero_l3f1594() {
    static UnkSpitStaticZero_l3f1594 s_value asm("g_unk_71726b5354");
    return s_value;
}

// The kind-5 block handed to the navigation (fn_71006f4b0c): an optional direction and the flags this start
// sets.
struct UnkSpitParams_l3f1594 {
    void** vtable;
    UnkVec3f dir;  // 0x8
    bool dir_set;     // 0x14
    u8 pad_0x15[7];
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    bool field_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;
};
static_assert(__builtin_offsetof(UnkSpitParams_l3f1594, kind) == 0x34, "layout");

struct UnkSpitTrackEntry_l3f1594 {
    u8 pad_0x0[0x170];
    u8 tracks[1];  // 0x170
};

struct UnkSpitDef_l3f1594 {
    u8 pad_0x0[0x30];
    void* track_key;  // 0x30
};

struct UnkSpitDefHolder_l3f1594 {
    UnkSpitDef_l3f1594* def;
};

struct UnkSpitOwner_l3f1594 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkStruct_7101176ea8* animation;  // 0x220
    u8 pad_0x228[0x70];
    void* navigation;  // 0x298
};

struct UnkSpitLink_l3f1594 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

// The callback's link and the weak handle of the object it is hooked to.
struct UnkSpitHook_l3f1594 {
    UnkSpitLink_l3f1594 link;  // 0x0
    UnkWeakHandle* handle;     // 0x18
};

struct UnkSpitStart_l3f1594 {
    void** vtable;
    UnkSpitOwner_l3f1594* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkSpitData* data;  // 0x28
    s32 state;          // 0x30
    s32 requested;      // 0x34
    UnkSpitHook_l3f1594 hook;  // 0x38
    f32 field_0x58;
    u8 pad_0x5c[0x18];
    base::math::CVector3D aim;  // 0x74
    bool aimed;                 // 0x80
    u8 pad_0x81[7];
    UnkRefObject* track_a;  // 0x88
    UnkRefObject* track_b;  // 0x90
    u8 pad_0x98[0x2c];
    u32 field_0xc4;
    u8 pad_0xc8[0x14];
    f32 timer;        // 0xdc
    f32 timer_1;      // 0xe0
    f32 next_timer;   // 0xe4
    u32 index;        // 0xe8
    UnkSpitVec3 target;  // 0xec
    UnkRefObject* field_0xf8;

    UnkSpitDefHolder_l3f1594* GetDefinition() {
        return reinterpret_cast<UnkSpitDefHolder_l3f1594* (*)(UnkSpitStart_l3f1594*)>(vtable[0xb0 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkSpitStart_l3f1594, field_0xf8) == 0xf8, "layout");

static inline void Release(UnkRefObject*& slot) {
    UnkRefObject* old = slot;
    if (old) {
        if (--old->ref_count == 0)
            old->vf_0x10();
        slot = nullptr;
    }
}

void fn_71003f1594(UnkSpitStart_l3f1594* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->state = 1;
    self->requested = 1;
    UnkStruct_7101176ea8* animation = nullptr;
    if (UnkSpitOwner_l3f1594* owner = self->owner) {
        animation = owner->animation;
        if (animation) {
            UnkSpitHook_l3f1594* hook = &self->hook;
            UnkWeakHandle* handle = hook->handle;
            UnkSpitLink_l3f1594* link = &hook->link;
            link->context = self;
            link->callback = fn_71003f4200;
            if (handle) {
                if (UnkWeakTarget* target = handle->target)
                    fn_710117b3f0(target, link);
                UnkWeakHandle* old = hook->handle;
                if (old) {
                    if (--old->ref_count == 0)
                        old->vf_0x10();
                    hook->handle = nullptr;
                }
            }
            fn_7100316018(reinterpret_cast<UnkWeakTarget*>(animation), &hook->handle);
            fn_710117b3e8(hook->handle ? hook->handle->target : nullptr, link);
        }
    }

    UnkVec3f angle = self->owner->xform.GetAngle();
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&self->owner->xform));
    UnkVec3f facing = m.m[2] >= 0.0f ? g_unk_7101649374 : g_unk_7101649380;
    void* track = fn_7101176d34(animation, self->GetDefinition()->def->track_key);
    if (track &&
        fn_710006ba58(static_cast<UnkSpitTrackEntry_l3f1594*>(track)->tracks, fn_7100d2b3a8())) {
        angle.y = -angle.y;
        facing = -facing;
    }
    if (self->owner) {
        if (void* navigation = self->owner->navigation) {
            UnkSpitParams_l3f1594 params;
            params.field_0x1c = false;
            params.field_0x20 = 0;
            params.field_0x28 = 0;
            params.field_0x30 = false;
            params.vtable = g_unk_7101a25b10 + 2;
            params.kind = 5;
            params.dir_set = true;
            params.field_0x3b = false;
            params.field_0x3d = false;
            params.dir = facing;
            params.field_0x38 = true;
            params.field_0x39 = true;
            fn_71006f4b0c(navigation, &params, 1);
        }
    }

    self->field_0x58 = -1.0f;
    u32 count = self->data->field_0x150;
    self->index = count ? (rand() & 0x7fff) % count : 0;
    Release(self->field_0xf8);
    self->timer = -1.0f;
    self->timer_1 = -1.0f;
    self->next_timer = -1.0f;
    if (self->owner) {
        if (UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))) {
            self->target.x = target->field_0x44;
            self->target.y = target->field_0x54;
            self->target.z = target->field_0x64;
        }
    }
    self->aim = g_unk_710164779c;
    self->aimed = false;
    Release(self->track_a);
    Release(self->track_b);
    self->field_0xc4 = SpitStaticZero_l3f1594().value;
    fn_71003f18f8(reinterpret_cast<UnkSpitAim_ovb0267*>(self), true, true,
                  reinterpret_cast<const UnkSpitVec3_ovb0267*>(&angle));
}
