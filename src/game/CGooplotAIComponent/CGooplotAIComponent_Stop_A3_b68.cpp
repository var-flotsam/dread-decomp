// CGooplotAIComponent unit (tools/tu_map.py): fn_710059aa2c stops the Gooplot. While the mode of the
// object at +0x147f0 (+0x6f0) is 0 or 1 it sets the flag at +0x168 of what fn_71004dad10 finds for the
// owner (+0x30); then it sets the owner's movement (+0x218) velocity (+0x10c) to the zero vector and its
// speed (+0x118) to that vector's length, applies it with fn_71010d08d4, sets the two flags of
// src/game/CGooplotAIComponent/CGooplotAIComponent_Reset_b49.cpp on the component at +0x228 and calls
// slot 0x180 of the owner's +0x1b8 with (1, false). Names and layouts are placeholders.
#include <math.h>

#include "base/math/UnkZeroVector3.h"
#include "game/CCoreXAIComponent.h"
#include "game/CGooplotAIComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;

struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
struct UnkFlagTarget;
extern char g_unk_710164a040[];
extern char g_unk_710164a048[];

struct UnkGooplotMovement_A3_b68 {
    u8 pad_0x0[0x10c];
    CVector3D velocity;
    f32 speed;
};

struct UnkGooplotSwitch_A3_b68 {
    void** vtable;
    void vf_0x180(u32 a, bool b) {
        reinterpret_cast<void (*)(UnkGooplotSwitch_A3_b68*, u32, bool)>(vtable[0x180 / 8])(this, a, b);
    }
};

struct UnkGooplotOwner_A3_b68 {
    u8 pad_0x0[0x1b8];
    UnkGooplotSwitch_A3_b68* field_0x1b8;
    u8 pad_0x1c0[0x218 - 0x1c0];
    UnkGooplotMovement_A3_b68* field_0x218;
    u8 pad_0x220[0x8];
    UnkFlagTarget* field_0x228;
};

struct UnkGooplotMode_A3_b68 {
    u8 pad_0x0[0x6f0];
    u32 field_0x6f0;
};

struct UnkGooplotAI_A3_b68 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_A3_b68* owner;
    u8 pad_0x38[0x147f0 - 0x38];
    UnkGooplotMode_A3_b68* field_0x147f0;
};

void fn_710059aa2c(UnkGooplotAI_A3_b68* self, void*) {
    if (self->field_0x147f0->field_0x6f0 <= 1) {
        if (char* p = fn_71004dad10(reinterpret_cast<UnkActor*>(self->owner)))
            p[0x168] = 1;
    }
    UnkGooplotMovement_A3_b68* movement = self->owner->field_0x218;
    // Copied as words that may alias anything (x and y, then z).
    typedef u32 __attribute__((may_alias)) word;
    const word* src = reinterpret_cast<const word*>(&g_unk_710164779c);
    word* dst = reinterpret_cast<word*>(&movement->velocity);
    word x = src[0];
    word y = src[1];
    dst[0] = x;
    dst[1] = y;
    dst[2] = src[2];
    const CVector3D& v = movement->velocity;
    movement->speed = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(self->owner->field_0x218),
                  reinterpret_cast<const UnkMovementVec_71010d08d4*>(&g_unk_710164779c), true);
    if (UnkFlagTarget* comp = self->owner->field_0x228) {
        fn_7101172780(comp, g_unk_710164a040);
        fn_7101172764(comp, g_unk_710164a048);
    }
    self->owner->field_0x1b8->vf_0x180(1, false);
}
