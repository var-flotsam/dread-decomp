// CTakumakuDashAttack's unit (tools/tu_map.py, unit 1075): the attack's start: after the base step, keeps
// the owner's Takumaku AI component (+0x30), resets the state, timers and flags, listens to the owner's
// animation (+0x220) through the link at +0x40, then faces the owner's navigation (+0x298) and movement
// (+0x218) along one of two constant directions (mirrored when the dash animation is a turn track).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkTakumakuDir_b58 {
    f32 x, y, z;
};
extern const UnkTakumakuDir_b58 g_unk_710164a7a0 __attribute__((visibility("hidden")));
extern const UnkTakumakuDir_b58 g_unk_710164a7ac __attribute__((visibility("hidden")));

bool fn_710006ba58(void* tracks, const base::reflection::CClass* cls);

// The facing request of kind 5 handed to the navigation (fn_71006f4b0c).
struct UnkTakumakuFacing_l6cea10 {
    void** vtable;
    UnkTakumakuDir_b58 dir;  // 0x8
    bool has_dir;            // 0x14
    u8 pad_0x15[7];
    bool field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    bool field_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;

    explicit UnkTakumakuFacing_l6cea10(const UnkTakumakuDir_b58& d) {
        field_0x1c = false;
        __builtin_memset(field_0x20, 0, 16);
        field_0x30 = false;
        vtable = g_unk_7101a25b10 + 2;
        kind = 5;
        has_dir = true;
        field_0x3b = false;
        field_0x3d = false;
        dir = d;
        field_0x38 = field_0x39 = true;
    }
};

// The facing request handed to the movement (fn_71010c7e6c).
struct UnkTakumakuTurn_l6cea10 {
    void** vtable;
    u32 field_0x8;
    bool has_0x8;  // 0xc
    u8 pad_0xd[3];
    f32 field_0x10;
    bool has_0x10;  // 0x14
    u8 pad_0x15[3];
    UnkTakumakuDir_b58 dir;  // 0x18
    bool has_dir;            // 0x24
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 kind;  // 0x38

    explicit UnkTakumakuTurn_l6cea10(const UnkTakumakuDir_b58& d) {
        has_0x8 = false;
        has_0x10 = false;
        vtable = g_unk_7101a24b80 + 2;
        kind = 0;
        __builtin_memset(field_0x28, 0, sizeof(field_0x28));
        dir = d;
        has_dir = true;
    }
};

struct UnkTakumakuEntry_l6cea10 {
    u8 pad_0x0[0x30];
    void* key;  // 0x30
};

struct UnkTakumakuInit_l6cea10;

// The listener the attack registers with its owner's animation.
struct UnkTakumakuLink_l6cea10 {
    void* field_0x0;
    UnkTakumakuInit_l6cea10* owner;          // 0x8
    void (*callback)(UnkTakumakuInit_l6cea10*);  // 0x10
};

void fn_71006d0e18(UnkTakumakuInit_l6cea10* self);

struct UnkTakumakuInit_l6cea10 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkReflObject* ai;  // 0x30
    s32 state;          // 0x38
    s32 next;           // 0x3c
    UnkTakumakuLink_l6cea10 link;  // 0x40
    UnkWeakHandle* handle;         // 0x58
    f32 countdown;                 // 0x60
    bool in_front;                 // 0x64
    u8 pad_0x65[3];
    f32 distance;     // 0x68
    f32 field_0x6c;   // 0x6c
    f32 field_0x70;   // 0x70
    bool field_0x74;  // 0x74
    bool started;     // 0x75
    bool field_0x76;  // 0x76

    UnkTakumakuEntry_l6cea10** vf_0xb0() {
        return reinterpret_cast<UnkTakumakuEntry_l6cea10** (*)(UnkTakumakuInit_l6cea10*)>(vtable[0xb0 / 8])(this);
    }
};

// The object when it is a CTakumakuAIComponent.
static inline UnkReflObject* AsTakumakuAI(UnkReflObject* object) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), fn_71006d3ba8()) ? object : nullptr;
}

// The direction pointing the other way.
static inline UnkTakumakuDir_b58 Mirrored(const UnkTakumakuDir_b58& d) {
    UnkTakumakuDir_b58 r = {-d.x, -d.y, -d.z};
    return r;
}

void fn_71006cea10(UnkTakumakuInit_l6cea10* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->ai = self->owner ? AsTakumakuAI(static_cast<UnkReflObject*>(self->owner->field_0x240)) : nullptr;
    UnkActor* owner = self->owner;
    void* animation = owner ? owner->field_0x220 : nullptr;
    void* navigation = owner ? owner->field_0x298 : nullptr;
    void* movement = owner ? owner->field_0x218 : nullptr;
    self->state = 0;
    self->next = 1;
    self->in_front = false;
    self->distance = 0.0f;
    self->field_0x6c = -1.0f;
    self->countdown = -1.0f;
    self->field_0x70 = -1.0f;
    if (animation) {
        self->link.owner = self;
        self->link.callback = fn_71006d0e18;
        if (UnkWeakHandle* handle = self->handle) {
            if (handle->target)
                fn_710117b3f0(handle->target, &self->link);
            ResetRef(&self->handle);
        }
        CAbilityGhostAura_AssignWeakHandleToRef(static_cast<UnkWeakTarget*>(animation), &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, &self->link);
    }
    self->field_0x74 = false;
    self->started = false;
    self->field_0x76 = false;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    UnkTakumakuDir_b58 dir = m.m[2] >= 0.0f ? g_unk_710164a7a0 : g_unk_710164a7ac;
    u8* entry = static_cast<u8*>(
        fn_7101176d34(static_cast<UnkStruct_7101176ea8*>(animation), (*self->vf_0xb0())->key));
    bool turn = entry && fn_710006ba58(entry + 0x170, fn_7100d2b3a8());
    UnkTakumakuDir_b58 facing_dir = turn ? Mirrored(dir) : dir;
    if (navigation) {
        UnkTakumakuFacing_l6cea10 facing(facing_dir);
        fn_71006f4b0c(navigation, &facing, 1);
    }
    if (movement) {
        UnkTakumakuTurn_l6cea10 turn_request(facing_dir);
        fn_71010c7e6c(static_cast<UnkEffectSink*>(movement), &turn_request, 0);
    }
}
