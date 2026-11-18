// CGooplotAIComponent unit (tools/tu_map.py unit 851): picks the action to run into the result's id
// and marks the result (+0x24, bit 2). In movement states 3 or 2 the movement gets a kind-2 request
// first; otherwise the animation's kinds 4 and 5 decide, and failing those the side the target is on
// against which way up the owner is. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CAnimationComponent.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

template <class T>
struct UnkOpt_l59bb18 {
    T value;
    bool has;
};

struct UnkVec3_l59bb18 {
    f32 x, y, z;
};

// The block sent to the movement (vtable g_unk_7101a2e1c0).
struct UnkMoveParams_l59bb18 {
    void** vtable;
    UnkOpt_l59bb18<u32> field_0x8;
    UnkOpt_l59bb18<f32> field_0x10;
    UnkOpt_l59bb18<UnkVec3_l59bb18> field_0x18;
    UnkOpt_l59bb18<UnkVec3_l59bb18> field_0x28;
    u32 kind;  // 0x38
};

struct UnkGooplotPickAI_l59bb18 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

struct UnkGooplotPick_l59bb18 {
    u64 id;
    u8 pad_0x8[0x1c];
    u32 flags;  // 0x24
};

static const u64 kActionLanding = 0x838e68291cefaf23ULL;
static const u64 kActionA = 0xbfa40b3858571829ULL;
static const u64 kActionB = 0x6a1b608cf60d5ea5ULL;

bool fn_710059bb18(UnkGooplotPickAI_l59bb18* self, UnkGooplotPick_l59bb18* out) {
    void* movement = self->owner->field_0x218;
    void* animation = self->owner->field_0x220;
    bool picked;
    if (movement && (fn_71010cc3a4(movement) == 3 || fn_71010cc3a4(movement) == 2)) {
        UnkMoveParams_l59bb18 params;
        params.field_0x8.has = false;
        params.field_0x10.has = false;
        __builtin_memset(&params.field_0x18, 0, 13);
        __builtin_memset(&params.field_0x28, 0, 13);
        params.vtable = g_unk_7101a2e1c0 + 2;
        params.kind = 2;
        fn_71010c7e6c(static_cast<UnkEffectSink*>(movement), &params, 1);
        out->id = kActionLanding;
        picked = true;
    } else if (!animation) {
        picked = false;
    } else if (fn_7101178e6c(static_cast<UnkTakumakuComponent*>(animation), 4)) {
        out->id = kActionA;
        picked = true;
    } else if (fn_7101178e6c(static_cast<UnkTakumakuComponent*>(animation), 5)) {
        out->id = kActionB;
        picked = true;
    } else if (!fn_7100d7c990(self->owner)) {
        picked = false;
    } else {
        f32 dx = fn_7100d7c990(self->owner)->field_0x44 - self->owner->field_0x44;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        // The owner's up axis (the matrix's second column) along world y.
        f32 axisX = m.m[1], axisY = m.m[5], axisZ = m.m[9];
        f32 up = axisX * 0.0f + axisY * 1.0f + axisZ * 0.0f;
        bool upsideDown = up < -0.001f;
        if (dx > 0.0f)
            out->id = upsideDown ? kActionA : kActionB;
        else
            out->id = upsideDown ? kActionB : kActionA;
        picked = true;
    }
    out->flags |= 2;
    return picked;
}
