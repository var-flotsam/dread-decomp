// CSluggerSpitAttack unit: the attack's start. After the base start it hooks the callback fn_71006c4214
// (link at +0x38, handle at +0x50) up to the owner's animation (+0x220), picks the facing from the owner's
// matrix (flipped, with the angles' y, when the attack's animation is a turn track), hands it to the owner's
// facing (+0x298) and effect sink (+0x218), drops the handle at +0xe0 and aims. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAttack.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern const UnkVec3f g_unk_710164a740 __attribute__((visibility("hidden")));  // facing right
extern const UnkVec3f g_unk_710164a74c __attribute__((visibility("hidden")));  // facing left

struct UnkSluggerOwner_l6c0b98 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    UnkEffectSink* sink;                // 0x218
    UnkStruct_7101176ea8* animation;    // 0x220
    u8 pad_0x228[0x298 - 0x228];
    void* facing;  // 0x298
};
static_assert(__builtin_offsetof(UnkSluggerOwner_l6c0b98, facing) == 0x298, "facing");

// An object and the function to call on it.
struct UnkSluggerLink_l6c0b98 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkSluggerStart_l6c0b98 {
    void** vtable;
    UnkSluggerOwner_l6c0b98* owner;  // 0x8
    u8 pad_0x10[0x20];
    u32 state;  // 0x30
    u8 pad_0x34[4];
    UnkSluggerLink_l6c0b98 link;  // 0x38
    UnkWeakHandle* handle;        // 0x50
    f32 field_0x58;
    u8 pad_0x5c[0xe0 - 0x5c];
    UnkRefObject* field_0xe0;

    // vtable slot 0xb0: the name of the attack's animation.
    const CStrId* AnimationName() {
        return reinterpret_cast<const CStrId* (*)(UnkSluggerStart_l6c0b98*)>(vtable[0xb0 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkSluggerStart_l6c0b98, field_0xe0) == 0xe0, "field_0xe0");

// A direction that may be unset.
struct UnkOptDir_l6c0b98 {
    UnkVec3f value;
    bool set;
};

// The kind-5 facing block for the owner's +0x298 (fn_71006f4b0c).
struct UnkFacingParams_l6c0b98 {
    void** vtable;
    UnkOptDir_l6c0b98 dir;  // 0x8
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

// The block for the owner's effect sink (fn_71010c7e6c).
struct UnkSinkParams_l6c0b98 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    UnkOptDir_l6c0b98 dir;  // 0x18
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
};

// The part of the string data read here: a 64-bit key at 0x30.
struct UnkStringKey_l6c0b98 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkSluggerSpitAttack_b46a2;
struct UnkSluggerSpitAttackReset;
void fn_71006c4214();
void fn_71006c0930(UnkSluggerSpitAttackReset* self);
void fn_71006c0e38(UnkSluggerSpitAttack_b46a2* self, bool mode, UnkVec3f* angles);
bool fn_710006ba58(void* tracks, const base::reflection::CClass* cls);

void fn_71006c0b98(UnkSluggerStart_l6c0b98* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkSluggerOwner_l6c0b98* owner = self->owner;
    self->state = 1;
    UnkStruct_7101176ea8* animation = owner ? owner->animation : nullptr;
    if (animation) {
        UnkSluggerLink_l6c0b98* link = &self->link;
        link->context = self;
        link->callback = fn_71006c4214;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        fn_7100316018(reinterpret_cast<UnkWeakTarget*>(animation), &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    UnkVec3f angles = self->owner->xform.GetAngle();

    const UnkMatrix44& m = self->owner->xform.GetMatrix();
    UnkVec3f dir = m.m[2] >= 0.0f ? g_unk_710164a740 : g_unk_710164a74c;
    u64 key = reinterpret_cast<const UnkStringKey_l6c0b98*>(self->AnimationName()->data)->key;
    u8* entry = static_cast<u8*>(fn_7101176d34(animation, reinterpret_cast<void*>(key)));
    bool turning = false;
    if (entry) {
        const base::reflection::CClass* turn_track = fn_7100d2b3a8();
        turning = fn_710006ba58(entry + 0x170, turn_track);
    }
    if (turning) {
        angles.y = -angles.y;
        dir.x = -dir.x;
        dir.y = -dir.y;
        dir.z = -dir.z;
    }

    void* facing = self->owner ? self->owner->facing : nullptr;
    if (facing) {
        UnkFacingParams_l6c0b98 params;
        params.field_0x1c = 0;
        params.field_0x20[0] = 0;
        params.field_0x20[1] = 0;
        params.field_0x30 = 0;
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x34 = 5;
        params.dir.set = true;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.dir.value = dir;
        params.field_0x38 = 1;
        params.field_0x39 = 1;
        fn_71006f4b0c(facing, &params, 1);
    }
    UnkEffectSink* sink = self->owner ? self->owner->sink : nullptr;
    if (sink) {
        UnkSinkParams_l6c0b98 params;
        params.field_0xc = 0;
        params.field_0x14 = 0;
        params.vtable = g_unk_7101a24b80 + 2;
        params.field_0x38 = 0;
        __builtin_memset(params.field_0x28, 0, 13);
        params.dir.value = dir;
        params.dir.set = true;
        fn_71010c7e6c(sink, &params, 0);
    }

    self->field_0x58 = -1.0f;
    if (UnkRefObject* p = self->field_0xe0) {
        if (--p->ref_count == 0)
            p->vf_0x10();
        self->field_0xe0 = nullptr;
    }
    fn_71006c0930(reinterpret_cast<UnkSluggerSpitAttackReset*>(self));
    fn_71006c0e38(reinterpret_cast<UnkSluggerSpitAttack_b46a2*>(self), 1, &angles);
}
