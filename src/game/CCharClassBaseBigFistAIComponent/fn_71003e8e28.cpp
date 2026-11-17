// CCharClassBaseBigFistAIComponent's unit (tools/tu_map.py unit 593): after the base character class's check
// (which wins when it answers), picks 5 or 4 by the other actor's melee component, then 2 when the actor's track
// allows crazy mode and the other actor is in front of it, or 1 when it allows pushing. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassAIComponent.h"
#include "game/CMeleeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// As src/game/CCharClassSluggerAIComponent/CCharClassSluggerAIComponent_b38.cpp declares them.
struct UnkSluggerPos {
    f32 x;
    f32 y;
    f32 z;
    UnkSluggerPos(const UnkSluggerPos& other);
};
UnkSluggerPos fn_71011008a8(void* ai, u32 index);

struct UnkBigFistOther_l3e8e28 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x2f8 - 0x68];
    void* melee;  // 0x2f8
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
u32 fn_71003e8e28(void* self, UnkActor* actor, UnkBigFistOther_l3e8e28* other) {
    u32 result = fn_7100414160();
    if (result)
        return result;
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(actor->field_0x220);
    void* ai = actor->field_0x240;
    result = 4;
    if (other && other->melee && fn_710092b6f8(other->melee))
        result = 5;
    if (!track)
        return result;
    if (!fn_710117b348(track, fn_7100ce9338())) {
        if (fn_710117b348(track, fn_7100bfca98()))
            return 1;
        return result;
    }
    f32 x = other->x;
    f32 y = other->y;
    f32 z = other->z;
    UnkSluggerPos pos = fn_71011008a8(ai, 0);
    f32 dx = x - pos.x;
    f32 dy = y - pos.y;
    f32 dz = z - pos.z;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    return dx * fx + dy * fy + dz * fz > 0.0f ? 2 : result;
}
