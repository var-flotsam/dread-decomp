// CXParasiteAIComponent unit (tools/tu_map.py): when the flag at +0x146ec is set and the prey (the weak
// handle at +0x14798) is there, the owner (+0x30) is placed (vtable slot 0xd0, with `true`) at the point at
// +0x15c4 of the prey's AI component (its +0x240), when it has one; otherwise state 7 (+0x146e0) runs
// fn_71006e77b8.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkTU1102.h"
#include "game/UnkActor.h"
#include "game/UnkXParasiteAI.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

// The prey's AI component (its actor's +0x240), as far as this source reads it: a point at +0x15c4.
struct UnkXParasitePreyAI {
    u8 pad_0x0[0x15c4];
    UnkVec3f point;  // 0x15c4
};

void fn_71006e77b8(UnkXParasiteAI* self);

void fn_71006e7754(UnkXParasiteAI* self) {
    if (self->field_0x146ec && self->field_0x14798 && self->field_0x14798->target) {
        if (auto* ai = static_cast<UnkXParasitePreyAI*>(Prey(self)->field_0x240))
            self->owner->vf_0xd0(&ai->point, true);
        return;
    }
    if (self->state == 7)
        fn_71006e77b8(self);
}

// Whether the parasite may act: reads the blackboard flag (into +0x146eb) first; false while that flag
// is set or the link chain leads to an object, else unless the state is 2 or 8.
bool fn_71006eb590(UnkXParasiteAI* self) {
    if (CBlackboard::CSection* section = fn_7101047270(&self->blackboard, g_unk_71726b7cb8))
        fn_710006d11c(section, &g_unk_717270b8e0, &self->field_0x146eb);
    bool busy = self->field_0x146eb || (self->field_0x14798 && self->field_0x14798->target);
    return !busy && self->state != 2 && self->state != 8;
}

// Ending the grab: when the link at +0x14790 leads to an object, its partner's (+0x398, +0x10) value at
// +0x2c0 is copied to +0x2bc, the object's component at +0x240 is told (vtable slot 0x410), the link is
// dropped and the flag at +0x147b9 set; without one, fn_71006e892c runs instead.
void fn_71006ea160(UnkXParasiteAI* self) {
    UnkWeakHandle* link = self->link;
    if (!link || !link->target) {
        fn_71006e892c(self);
        return;
    }
    UnkXParasiteGrabbed* object = Grabbed(link);
    if (object->partner_link && object->partner_link->partner) {
        UnkXParasitePartner* partner = object->partner_link->partner;
        // Stored through a may_alias type: the original reloads the link afterwards.
        typedef u32 __attribute__((may_alias)) AliasU32;
        *reinterpret_cast<AliasU32*>(&partner->field_0x2bc) = partner->field_0x2c0;
        object = Grabbed(self->link);
    }
    if (object->listener)
        object->listener->Notify(self);
    if (UnkWeakHandle* l = self->link) {
        if (--l->ref_count == 0)
            l->vf_0x10();
        self->link = nullptr;
    }
    self->released = true;
}
