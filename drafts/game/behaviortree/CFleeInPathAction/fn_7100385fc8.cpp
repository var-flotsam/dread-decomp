// behaviortree::CFleeInPathAction's unit (tools/tu_map.py unit 483): the action's update. It fails (6)
// without its two targets or once stopped; otherwise it marks the flee reached (+0x61) when forced or when
// the owner's movement is within the radius of the point (seen from above), then hands the navigation
// (+0x48) a cleared kind-5 block, unless it is already in that state, and sends the owner's animation the
// name at +0x68. It returns 5 when done and finishing, else 2. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU1730.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkFleeMovement_l385fc8 {
    u8 pad_0x0[0x974];
    f32 x;  // 0x974
    f32 y;  // 0x978
};

struct UnkFleeOwner_l385fc8 {
    u8 pad_0x0[0x218];
    UnkFleeMovement_l385fc8* movement;  // 0x218
    void* animation;  // 0x220
};

struct UnkFleeStateKind_l385fc8 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkFleeState_l385fc8 {
    u8 pad_0x0[0x10];
    UnkFleeStateKind_l385fc8* kind;  // 0x10
};

struct UnkFleeStates_l385fc8 {
    UnkFleeState_l385fc8* current;  // 0x0
    u8 pad_0x8[0x20];
    void* field_0x28;  // 0x28
};

struct UnkFleeNavigation_l385fc8 {
    u8 pad_0x0[0xb0];
    UnkFleeStates_l385fc8* states;  // 0xb0
};

struct UnkFleeInPathAction_l385fc8 {
    u8 pad_0x0[0x18];
    UnkFleeOwner_l385fc8* owner;  // 0x18
    u8 pad_0x20[0x20];
    void* field_0x40;  // 0x40
    UnkFleeNavigation_l385fc8* navigation;  // 0x48
    CVector3D point;  // 0x50
    f32 radius;  // 0x5c
    u8 pad_0x60;
    bool reached;  // 0x61
    bool forced;  // 0x62
    bool stopped;  // 0x63
    bool finish;  // 0x64
    bool send_name;  // 0x65
    u8 pad_0x66[2];
    CStrId name;  // 0x68
};

void fn_7100385798(UnkFleeInPathAction_l385fc8* self);

// Whether the navigation is already in a kind-5 state that has its target.
static inline bool InKind5(const UnkFleeNavigation_l385fc8* navigation) {
    UnkFleeStates_l385fc8* states = navigation->states;
    UnkFleeState_l385fc8* current = states->current;
    return current && current->kind && current->kind->kind == 5 && states->field_0x28;
}

// Whether the movement is within the radius of the point on both axes, seen from above.
static inline bool InRadius(const UnkFleeInPathAction_l385fc8* self, const UnkFleeMovement_l385fc8* movement) {
    CVector2D p;
    fn_710007b430(&p, &self->point);
    f32 radius = self->radius;
    return radius + p.x >= movement->x && p.x - radius <= movement->x && radius + p.y >= movement->y &&
           p.y - radius <= movement->y;
}

// 5 when the flee is reached and the action finishes there, else 2 (running).
static inline u32 Result(const UnkFleeInPathAction_l385fc8* self) {
    if (self->finish && self->reached)
        return 5;
    return 2;
}

u32 fn_7100385fc8(UnkFleeInPathAction_l385fc8* self) {
    if (!self->field_0x40 || !self->navigation)
        return 6;
    self->reached = false;
    UnkFleeMovement_l385fc8* movement = self->owner->movement;
    if (movement && fn_71010cc3a4(movement) != 4)
        fn_7100385798(self);
    if (self->stopped)
        return 6;
    if (self->forced)
        self->reached = true;
    if (movement && fn_71010cc3a4(movement) == 1 && InRadius(self, movement)) {
        self->reached = true;
    } else if (!self->reached) {
        return Result(self);
    }
    {
        if (UnkFleeNavigation_l385fc8* navigation = self->navigation) {
            if (!InKind5(navigation)) {
                UnkParams params;
                params.field_0x1c = 0;
                __builtin_memset(params.field_0x20, 0, 16);
                params.field_0x30 = 0;
                params.vtable = g_unk_7101a25b10 + 2;
                __builtin_memset(params.field_0x8, 0, 13);
                params.field_0x3d = 0;
                params.field_0x34 = 5;
                params.pad_0x38 = 1;
                params.field_0x39 = 1;
                params.pad_0x3a = 1;
                params.field_0x3b = 1;
                fn_71006f4b0c(navigation, &params, 1);
                if (self->send_name && self->name.data != &g_unk_7101d47108->empty) {
                    if (UnkFleeOwner_l385fc8* owner = self->owner) {
                        if (void* animation = owner->animation)
                            base::reflection::refl_EmitU32(animation, self->name.data->Key(), 0xf4061ff25a2163d6, 1,
                                                           &g_unk_710164b958, &g_unk_7172769740);
                    }
                }
            }
        }
    }
    return Result(self);
}
