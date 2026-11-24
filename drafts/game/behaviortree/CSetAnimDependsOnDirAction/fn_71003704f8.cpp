// behaviortree::CSetAnimDependsOnDirAction's unit (tools/tu_map.py, unit 452): the node's update. With its flag
// at +0x88 it plays the animation it names (as CSetAnimAction does, with a second name unless +0x1e1 is set);
// otherwise it waits while the owner turns (its enemy movement in state 4, or its animation on a turn track)
// and starts the turn (fn_710036f938) once it is done. Names and layouts are placeholders.
#include "../../../../src/game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU57.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

// A name built from a string id (fn_710007eb6c).
struct UnkAnimName_l3704f8 : UnkBufferString<0x100, g_unk_7101a194b8> {
    explicit UnkAnimName_l3704f8(const CStrId* id) { CRntString_AssignStrId(Str(), id); }
};

struct UnkDirOwner_l3704f8 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0xa0];
    void* field_0x2c8;
};

struct UnkSetAnimDependsOnDir_l3704f8 {
    u8 pad_0x0[0x18];
    UnkDirOwner_l3704f8* owner;  // 0x18
    u8 pad_0x20[0x10];
    CStrId name;                // 0x30
    CStrId name_property;       // 0x38
    CStrId alt_name;            // 0x40
    CStrId alt_name_property;   // 0x48
    u8 pad_0x50[0x28];
    bool field_0x78;
    bool field_0x79;
    u8 pad_0x7a[0xa];
    s32 number;  // 0x84
    bool play;   // 0x88
    u8 pad_0x89;
    bool use_movement;  // 0x8a
    u8 pad_0x8b;
    bool use_turn_track;  // 0x8c
    u8 pad_0x8d[0x1e1 - 0x8d];
    bool keep_name;   // 0x1e1
    bool turning;     // 0x1e2
    bool waited;      // 0x1e3
};
static_assert(__builtin_offsetof(UnkSetAnimDependsOnDir_l3704f8, waited) == 0x1e3, "layout");

// Signatures as other files declare them.
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;

void fn_710036f938(UnkSetAnimDependsOnDir_l3704f8* self);

// Resolves `property` into `name` when it is set, then appends the number when there is one.
static inline void ResolveName(UnkSetAnimDependsOnDir_l3704f8* self, CStrId* property, UnkAnimName_l3704f8* name) {
    if (property->data != &g_unk_7101d47108->empty)
        CBehavior_ReadStringProperty(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), property,
                      reinterpret_cast<UnkInlineString_A2_b72*>(name));
    if (self->number >= 0)
        fn_710007f634(name->Str(), self->number);
}

// Whether the owner's enemy movement is turning (state 4).
static inline bool MovementTurning(UnkSetAnimDependsOnDir_l3704f8* self, UnkReflObject* enemy) {
    return enemy && self->use_movement && fn_71010cc3a4(enemy) == 4;
}

u32 fn_71003704f8(UnkSetAnimDependsOnDir_l3704f8* self) {
    if (self->play) {
        if (!self->field_0x78)
            return 5;
        UnkAnimName_l3704f8 name(&self->name);
        ResolveName(self, &self->name_property, &name);
        if (!self->keep_name) {
            {
                UnkAnimName_l3704f8 alt(&self->alt_name);
                CRntString_Assign(name.Str(), alt.Str());
            }
            ResolveName(self, &self->alt_name_property, &name);
        }
        if (self->owner) {
            UnkAnimationComponent* animation = self->owner->animation;
            if (animation) {
                const CStrId* current = fn_7101177ae8(animation, 0xf4061ff25a2163d6);
                ++current->data->reads;
                if (!CRntString_KeysDiffer(&current->data->String(), name.Str()) &&
                    !fn_71011782ec(animation, 0xf4061ff25a2163d6))
                    return 2;
            }
        }
        if (self->field_0x79 && self->owner && self->owner->field_0x2c8)
            return fn_71007f9f20(self->owner->field_0x2c8) ? 2 : 5;
        return 5;
    }

    UnkDirOwner_l3704f8* owner = self->owner;
    UnkAnimationComponent* animation = owner ? owner->animation : nullptr;
    UnkReflObject* movement = owner->movement;
    UnkReflObject* enemy =
        movement && fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) ? movement : nullptr;
    if (MovementTurning(self, enemy) ||
        (animation && self->use_turn_track && fn_710117b348(animation, fn_7100d2b3a8()))) {
        if (MovementTurning(self, enemy))
            self->turning = true;
        else
            self->waited = true;
    } else {
        self->turning = false;
        self->waited = false;
    }
    if (!self->turning && !self->waited)
        fn_710036f938(self);
    return 2;
}
