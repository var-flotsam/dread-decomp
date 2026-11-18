// CBigFistChangeToFleeAction unit. fn_71003e3ab4, the action's run: 5 once the flee message came (+0x60);
// 2 while the animation (+0x38) is on an ignore-flee track or the movement says so; otherwise stops layers 11
// and 6, plays layer 12 and, when the animation is on the combat track (and neither flee nor patrol), asks
// fn_7101110964 for the flee (6 when it says no), links the message callback (link at +0x40, handle at
// +0x58) to the owner's +0x220 and hands the owner's +0x298 a block facing the owner's z axis along x.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern __attribute__((visibility("hidden"))) void* g_unk_71726c0b38;
// The two facings along x.
extern const UnkVec3f g_unk_7101649308 __attribute__((visibility("hidden")));
extern const UnkVec3f g_unk_7101649314 __attribute__((visibility("hidden")));

struct UnkBigFistAction;
struct UnkBigFistMsg;
bool fn_71003e3f5c(UnkBigFistAction* self, const UnkBigFistMsg* msg);
bool fn_7101110964(void* ai, void** id, const u64* key, u32* out, f32 a, f32 b);

// The id table's slot this action reads (not yet a member of UnkGameManagerIdTable).
struct UnkFleeIdTable_l3e3ab4 {
    u8 pad_0x0[0x10c0];
    CStrId field_0x10c0;
};

inline const CStrId& FleeId_l3e3ab4() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return reinterpret_cast<const UnkFleeIdTable_l3e3ab4*>(&s_table)->field_0x10c0;
}

// A direction that may be unset: three floats and a flag, cleared as 13 bytes.
struct UnkOptDir_l3e3ab4 {
    UnkVec3f value;
    bool set;

    void Clear() { __builtin_memset(this, 0, 13); }
};

// The kind-5 block with its direction at +0x8.
struct UnkDirParams_l3e3ab4 {
    void** vtable;
    UnkOptDir_l3e3ab4 dir;
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

// An object and the function to call on it, as the animation calls back.
struct UnkBigFistLink_l3e3ab4 {
    u8 pad_0x0[0x8];
    void* object;
    void* function;
};

struct UnkBigFistOwner_l3e3ab4 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    void* movement;          // 0x218
    UnkWeakTarget* tracked;  // 0x220
    u8 pad_0x228[0x298 - 0x228];
    void* field_0x298;
};
static_assert(__builtin_offsetof(UnkBigFistOwner_l3e3ab4, field_0x298) == 0x298, "layout");

struct UnkBigFistChangeToFlee_l3e3ab4 {
    u8 pad_0x0[0x18];
    UnkBigFistOwner_l3e3ab4* owner;  // 0x18
    u8 pad_0x20[0x10];
    void* ai;                              // 0x30
    UnkAnimationComponent* animation;      // 0x38
    UnkBigFistLink_l3e3ab4 link;           // 0x40
    UnkWeakHandle* handle;                 // 0x58
    bool fled;                             // 0x60
};

// Asks the AI whether it can flee under the id's key.
static inline bool CanFlee(void* ai, const CStrId& id) {
    u64 key = id.data->Key();
    u32 result = 0;
    return fn_7101110964(ai, &g_unk_71726c0b38, &key, &result, -1.0f, 0.0f);
}

static inline bool OnTrack(UnkAnimationComponent* animation, const base::reflection::CClass* track) {
    return fn_710117b348(animation, track);
}

u32 fn_71003e3ab4(UnkBigFistChangeToFlee_l3e3ab4* self) {
    if (self->fled)
        return 5;
    if (OnTrack(self->animation, fn_7100c2cc08()) || fn_71010cab0c(self->owner->movement))
        return 2;
    fn_7101178e08(reinterpret_cast<UnkActorTarget*>(self->animation), 11, false, false, -1.0f);
    fn_7101178e08(reinterpret_cast<UnkActorTarget*>(self->animation), 6, false, false, -1.0f);
    fn_710117662c(reinterpret_cast<UnkActorTarget*>(self->animation), 12, 0, false, false, -1.0f);
    if (OnTrack(self->animation, fn_7100c19f48()) || OnTrack(self->animation, fn_7100c4d378()) ||
        !OnTrack(self->animation, fn_7100c1181c()))
        return 5;

    if (!CanFlee(self->ai, FleeId_l3e3ab4()))
        return 6;

    UnkBigFistLink_l3e3ab4* link = &self->link;
    link->object = self;
    link->function = reinterpret_cast<void*>(fn_71003e3f5c);
    UnkWeakTarget* tracked = self->owner->tracked;
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target)
            fn_710117b3f0(handle->target, link);
        ResetRef(&self->handle);
    }
    if (tracked) {
        fn_7100316018(tracked, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }

    UnkBigFistOwner_l3e3ab4* owner = self->owner;
    if (owner->field_0x298) {
        UnkDirParams_l3e3ab4 params;
        params.dir.Clear();
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x34 = 5;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        const UnkMatrix44& m = owner->xform.GetMatrix();
        params.dir.value = m.m[2] > 0.0f ? g_unk_7101649308 : g_unk_7101649314;
        params.dir.set = true;
        fn_71006f4b0c(self->owner->field_0x298, &params, 1);
    }
    return 2;
}
