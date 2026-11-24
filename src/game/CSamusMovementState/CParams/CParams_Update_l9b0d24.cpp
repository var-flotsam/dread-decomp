// CSamusMovementState::CParams's unit (tools/tu_map.py): a movement state's update: it counts the
// time, applies the cached impulse once, runs its own step and, while the owner's grapple beam gun
// is in state 3, reports a transition to its target. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CGunComponent.h"
#include "game/UnkDesc16_l9b0d24.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1730.h"
#include "types.h"

extern void* g_unk_7101a86df8[];

struct UnkSamusImpulseParams_b0105;
struct UnkSamusMovementTarget_b0105;
bool CSamusMovementState_ApplyOffsetToTarget(UnkSamusImpulseParams_b0105* self, UnkSamusMovementTarget_b0105* target);

struct UnkAirTarget_B71a6;
struct UnkAirRequest_B71a6;

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_l9b0d24 {
    s64 id;
    UnkDesc16_l9b0d24 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_l9b0d24(UnkDesc16_l9b0d24 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_l9b0d24()
        : UnkMoveTransition_l9b0d24(*reinterpret_cast<const UnkDesc16_l9b0d24*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l9b0d24) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_l9b0d24 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with this state's vtable.
struct UnkGrappleParams_l9b0d24 {
    void* vtable;
    UnkMoveTransition_l9b0d24 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l9b0d24 field_0x3c;
    u32 field_0x44;

    UnkGrappleParams_l9b0d24() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a86df8) + 0x10;
    }
};
static_assert(sizeof(UnkGrappleParams_l9b0d24) == 0x48, "layout");

struct UnkGrappleMode_l9b0d24 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};
struct UnkGrappleTarget_l9b0d24 {
    u8 pad_0x0[0x490];
    UnkGrappleMode_l9b0d24* field_0x490;
};
struct UnkGrappleLink_l9b0d24 {
    u8 pad_0x0[0x10];
    UnkGrappleTarget_l9b0d24* field_0x10;
};
struct UnkGrappleGun_l9b0d24 {
    u8 pad_0x0[0x1888];
    UnkGrappleLink_l9b0d24* field_0x1888;
};

struct UnkSamusOwner_l9b0d24 {
    u8 pad_0x0[0x2a8];
    UnkComponent_710035131c* field_0x2a8;
};

struct UnkSamusState_l9b0d24;
struct UnkSamusStateVtable_l9b0d24 {
    void* pad_0x0[34];
    void (*vf_0x110)(UnkSamusState_l9b0d24* self, bool flag);
    void (*vf_0x118)(UnkSamusState_l9b0d24* self);
};

struct UnkSamusState_l9b0d24 {
    UnkSamusStateVtable_l9b0d24* vtable;
    UnkAirTarget_B71a6* target;  // 0x8
    u32 state;                   // 0x10
    u8 pad_0x14[0x1c];
    UnkSamusOwner_l9b0d24* owner;               // 0x30
    UnkSamusMovementTarget_b0105* movement;     // 0x38
    u8 pad_0x40[0x108];
    f32 time;                                   // 0x148
    UnkSamusImpulseParams_b0105* impulse;       // 0x150
    bool impulse_applied;                       // 0x158
};
static_assert(__builtin_offsetof(UnkSamusState_l9b0d24, time) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkSamusState_l9b0d24, impulse_applied) == 0x158, "layout");

// The owner's grapple beam gun, when it has one.
static UnkGrappleGun_l9b0d24* GrappleGun(UnkSamusOwner_l9b0d24* owner) {
    UnkComponent_710035131c* c = owner->field_0x2a8;
    if (!c)
        return nullptr;
    auto* obj = static_cast<base::reflection::UnkReflObject*>(fn_710083cd30(c));
    if (!obj || !fn_710009d50c(obj->vf_0x0(), fn_7100832034()))
        return nullptr;
    return reinterpret_cast<UnkGrappleGun_l9b0d24*>(obj);
}

void fn_71009b0d24(void* object, void* arg) {
    UnkSamusState_l9b0d24* self = static_cast<UnkSamusState_l9b0d24*>(object);
    self->time = *static_cast<const f32*>(arg) + self->time;
    if (!self->impulse_applied && self->impulse) {
        CSamusMovementState_ApplyOffsetToTarget(self->impulse, self->movement);
        self->impulse_applied = true;
    }
    self->vtable->vf_0x118(self);
    if (self->state != 5) {
        if (UnkGrappleGun_l9b0d24* gun = GrappleGun(self->owner)) {
            UnkGrappleLink_l9b0d24* link = gun->field_0x1888;
            if (link && link->field_0x10 && link->field_0x10->field_0x490 &&
                link->field_0x10->field_0x490->field_0x10 == 3) {
                UnkGrappleParams_l9b0d24 params;
                fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
            }
        }
    }
    self->vtable->vf_0x110(self, false);
}
