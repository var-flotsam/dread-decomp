// behaviortree::CVulkranChaseAction's unit (tools/tu_map.py): the action's status. 7 unless the owner
// (+0x18) has a character class (+0x220), the flag at +0x59 is set and the one at +0x5a clear; then 3
// while the class plays the first or the second of the animations named by the object at +0x38
// (fn_71006dac28, fn_71006dac34: each a slot holding a record whose +0x30 is the animation id;
// fn_710117ae38), else 7. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkVulkranAnim_b65a4 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkVulkranAnims_b65a4;
UnkVulkranAnim_b65a4** fn_71006dac28(UnkVulkranAnims_b65a4* anims);
UnkVulkranAnim_b65a4** fn_71006dac34(UnkVulkranAnims_b65a4* anims);

struct UnkVulkranChase_b65a4 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x18];
    UnkVulkranAnims_b65a4* anims;  // 0x38
    u8 pad_0x40[0x19];
    bool field_0x59;
    bool field_0x5a;
};

s32 fn_71006d81d4(UnkVulkranChase_b65a4* self) {
    UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (cls && self->field_0x59 && !self->field_0x5a) {
        if (fn_710117ae38(cls, (*fn_71006dac28(self->anims))->id, false) ||
            fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220),
                          (*fn_71006dac34(self->anims))->id, false))
            return 3;
    }
    return 7;
}
