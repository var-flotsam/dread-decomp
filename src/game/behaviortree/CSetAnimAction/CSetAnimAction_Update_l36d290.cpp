// behaviortree::CSetAnimAction's unit (tools/tu_map.py, unit 450): the node's update. Without its flag at +0x60
// it fails (6); otherwise it builds the animation name (the id at +0x30, overridden by the blackboard value the
// id at +0x38 names, with the number at +0x4c appended when set) and, when the owner's animation component plays
// another one and refuses to switch, returns 2. Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkTU57.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;

// A name built from a string id (fn_710007eb6c).
struct UnkAnimName_l36d290 : UnkBufferString<0x100, g_unk_7101a194b8> {
    explicit UnkAnimName_l36d290(const CStrId* id) { CRntString_AssignStrId(Str(), id); }
};

struct UnkSetAnimOwner_l36d290 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0xa0];
    void* field_0x2c8;
};

struct UnkSetAnimAction_l36d290 {
    u8 pad_0x0[0x18];
    UnkSetAnimOwner_l36d290* owner;  // 0x18
    u8 pad_0x20[0x10];
    CStrId name;           // 0x30
    CStrId name_property;  // 0x38
    u8 pad_0x40[2];
    bool field_0x42;
    bool field_0x43;
    u8 pad_0x44[8];
    s32 number;  // 0x4c
    u8 pad_0x50[0x10];
    bool field_0x60;
};

// Signatures as other files declare them.
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;

u32 CSetAnimAction_WaitForAnimEnd(UnkSetAnimAction_l36d290* self) {
    if (!self->field_0x60)
        return 6;
    if (!self->field_0x42)
        return 5;
    UnkAnimName_l36d290 name(&self->name);
    if (self->name_property.data != &g_unk_7101d47108->empty)
        CBehavior_ReadStringProperty(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->name_property,
                      reinterpret_cast<UnkInlineString_A2_b72*>(&name));
    if (self->number >= 0)
        fn_710007f634(name.Str(), self->number);
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
    if (self->field_0x43 && self->owner && self->owner->field_0x2c8)
        return fn_71007f9f20(self->owner->field_0x2c8) ? 2 : 5;
    return 5;
}
