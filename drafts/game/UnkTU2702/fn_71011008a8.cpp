// Unit 2702: reads a 3-float anchor position from a component's owner, preferring the owner's
// linked object and then a looked-up target. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkAnchorLinked {
    u8 pad_0x290[0x290];
    f32 x_0x290;
    u8 pad_0x294[0xc];
    f32 y_0x2a0;
    u8 pad_0x2a4[0xc];
    f32 z_0x2b0;
};

struct UnkAnchorOwner {
    u8 pad_0x0[0x44];
    f32 x_0x44;
    u8 pad_0x48[0xc];
    f32 y_0x54;
    u8 pad_0x58[0xc];
    f32 z_0x64;
    u8 pad_0x68[0x1f8 - 0x68];
    UnkAnchorLinked* linked_0x1f8;
    u8 pad_0x200[0x220 - 0x200];
    UnkAnimationComponent* anim_0x220;
    void* target_0x228;
};

struct UnkAnchorGlobalRec {
    u8 pad_0x0[0x30];
    u64 id_0x30;
};

struct UnkAnchorGlobals {
    u8 pad_0x0[0xd60];
    UnkAnchorGlobalRec* rec_0xd60;
};

struct UnkAnchorFlags {
    u8 pad_0x0[0x40a];
    u8 flag_0x40a;
};

struct UnkAnchorComponent {
    u8 pad_0x0[0x30];
    UnkAnchorOwner* owner_0x30;
    u8 pad_0x38[0x1610 - 0x38];
    UnkAnchorFlags* flags_0x1610;
};

inline UnkAnchorGlobals* AnchorGlobals() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return (UnkAnchorGlobals*)&s_table;
}

// NON_MATCHING(minor): in the flag-clear path the owner reload and the two zero stores of `found` are scheduled in another order
UnkVec3f fn_71011008a8(UnkAnchorComponent* self, bool preferLinked) {
    UnkAnchorOwner* owner = self->owner_0x30;
    UnkVec3f result(owner->x_0x44, owner->y_0x54, owner->z_0x64);
    if (preferLinked && owner->linked_0x1f8) {
        UnkAnchorLinked* l = owner->linked_0x1f8;
        result = UnkVec3f(l->x_0x290, l->y_0x2a0, l->z_0x2b0);
    }
    UnkVec3f found;
    void* target;
    if (self->flags_0x1610->flag_0x40a == 0) {
        UnkAnimationComponent* anim = owner->anim_0x220;
        if (!anim) return result;
        if (!fn_710117b348(anim, fn_7100c649f8())) return result;
        __builtin_memset(&found, 0, sizeof(found));
        target = self->owner_0x30->target_0x228;
    } else {
        found = UnkVec3f(0, 0, 0);
        target = owner->target_0x228;
    }
    if (target) {
        u64 id = AnchorGlobals()->rec_0xd60->id_0x30;
        if (fn_710117364c(target, &id, (UnkGrappleAnswer_b50*)&found)) result = found;
    }
    return result;
}
