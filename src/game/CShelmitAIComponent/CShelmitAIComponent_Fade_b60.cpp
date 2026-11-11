// CShelmitAIComponent's unit (tools/tu_map.py): applies a fade value. When the value is 1 (within 0.001)
// and the owner's (+0x30) timeline (+0x2e8) has the entry g_unk_710164a718 (fn_7100a2c13c), that id is
// set to the all-flags mask (as CShelmitAIComponent_Timeline_b59.cpp); then the square of the value (not
// below 0) goes to all four components of slot 0 of the owner's +0x228 object's (+0x50) slot table
// (+0x138, fn_71001c2764). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/math/CVector4D.h"
#include "game/CShelmitAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkSlots.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkShelmitFadeKey_b60a4;
extern UnkShelmitFadeKey_b60a4 g_unk_710164a718;  // reached through the GOT

struct UnkShelmitFadeTarget_b60a4 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkShelmitFadeShape_b60a4 {
    u8 pad_0x0[0x50];
    UnkShelmitFadeTarget_b60a4* target;
};

struct UnkShelmitFade_b60a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

void fn_71006badf4(UnkShelmitFade_b60a4* self, f32 value) {
    f32 d = value - 1.0f;
    if (d + 0.001f >= 0.0f && d - 0.001f <= 0.0f && self->owner) {
        auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        if (timeline && fn_7100a2c13c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a718))) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a718), &mask.id, false, -1.0f);
        }
    }
    if (self->owner) {
        if (auto* shape = static_cast<UnkShelmitFadeShape_b60a4*>(self->owner->field_0x228)) {
            if (UnkShelmitFadeTarget_b60a4* target = shape->target) {
                f32 sq = value * value;
                f32 s = sq <= 0.0f ? 0.0f : sq;
                base::math::CVector4D v;
                v.x = s;
                v.y = s;
                v.z = s;
                v.w = s;
                fn_71001c2764(target->slots, 0, 1, &v, 1, true);
            }
        }
    }
}
