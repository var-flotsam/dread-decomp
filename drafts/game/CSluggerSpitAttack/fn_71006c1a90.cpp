// CSluggerSpitAttack unit: the point the spit aims at. The AI component's target point when it has one,
// else the target's position, lowered by 50 when the owner is above a target that is not on a surface
// and shifted 20 sideways along the target's facing angle. Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

bool fn_71009e12d0(void*);

// An optional target point (unaligned inside the AI component).
struct __attribute__((packed)) UnkSluggerTargetPoint {
    bool set;
    u8 pad_0x1[2];
    f32 x, y, z;  // 0x3
};

struct UnkSluggerAI {
    u8 pad_0x0[0x14699];
    UnkSluggerTargetPoint target;  // 0x14699
};

struct UnkSluggerOwner {
    u8 pad_0x0[0x240];
    UnkSluggerAI* field_0x240;
};

struct UnkSluggerTarget {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x208 - 0x94];
    void* field_0x208;
    void* field_0x210;
};
static_assert(__builtin_offsetof(UnkSluggerTarget, field_0x208) == 0x208, "layout");

struct UnkSluggerSpitAttack_Aim {
    u8 pad_0x0[8];
    UnkActor* owner;
};

UnkVec3f fn_71006c1a90(UnkSluggerSpitAttack_Aim* self) {
    UnkVec3f result = reinterpret_cast<const UnkVec3f&>(g_unk_710164779c);
    UnkActor* owner = self->owner;
    UnkSluggerAI* ai = static_cast<UnkSluggerAI*>(owner->field_0x240);
    if (ai && ai->target.set) {
        result = *reinterpret_cast<const UnkVec3f*>(&ai->target.x);
    } else if (owner) {
        UnkSluggerTarget* target = reinterpret_cast<UnkSluggerTarget*>(fn_7100d7c990(owner));
        if (target) {
            result = target->xform.GetTranslation();
            f32 x = result.x;
            f32 y = result.y;
            void* floor = target->field_0x208;
            void* ground = target->field_0x210;
            if (floor && fn_71009e12d0(floor)) {
                if (self->owner->field_0x54 < reinterpret_cast<UnkActor*>(target)->field_0x54)
                    result.y = y - 50.0f;
                result.x = x + (target->xform.GetAngle().y > 0.0f ? 20.0f : -20.0f);
            } else if (!ground) {
                result.y = y - 50.0f;
            }
        }
    }
    return result;
}
