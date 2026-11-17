// behaviortree::CPoisonFlyOnSamusDetectedAction's unit (tools/tu_map.py): the update. Counts the delay down; once
// a turn animation was sent and the delay is over, it waits for that animation to finish. Otherwise it picks one of
// two animations by whether the target lies on the side the owner faces and sends it with mode 0 (UnkSpitSend,
// src/game_s2/UnkSpitSend.h). Returns 2 while running, 5 when done. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkPoisonFlyDetected_l6354a0 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0xc];
    f32 delay;  // 0x2c
    u64 anim;   // 0x30: the animation sent, or ~0 for none
};
static_assert(__builtin_offsetof(UnkPoisonFlyDetected_l6354a0, anim) == 0x30, "layout");

static inline f32 Sign(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

u32 fn_71006354a0(UnkPoisonFlyDetected_l6354a0* self, f32 dt) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg same_mode;
    base::reflection::UnkReflTypeIdArg same_mask;
    base::reflection::UnkReflTypeIdArg other_mode;
    base::reflection::UnkReflTypeIdArg other_mask;
start:
    const u64 kind = 0xf4061ff25a2163d6;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (self->anim != ~0ull && self->delay < 0.0f) {
        if (fn_7101177ae8(animation, kind)->data->Key() != self->anim || fn_71011782ec(animation, kind))
            return 5;
    }
    self->delay -= dt;
    if (self->anim != ~0ull)
        return 2;

    UnkActor* target = fn_7100d7c990(self->owner);
    f32 facing = Sign(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2]);
    f32 side = Sign(target->field_0x44 - self->owner->field_0x44);
    if (facing == side) {
        self->anim = 0x1720955b012a5867;
        same_mode.id = 0;
        same_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, self->anim, kind, &same_mode, &g_unk_710164b958, &g_unk_7172769740, &same_mask);
    } else {
        self->anim = 0x1e4b917f54b4f6f2;
        other_mode.id = 0;
        other_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, self->anim, kind, &other_mode, &g_unk_710164b958, &g_unk_7172769740, &other_mask);
    }
    return 5;
}
