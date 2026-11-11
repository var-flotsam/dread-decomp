// CCharClassGroundShockerAIComponent's unit (tools/tu_map.py): one of two tuning values
// (g_unk_71726e85b8 +0x24 or +0x28), chosen by whether the actor's animation (+0x220) is in
// state 11 (fn_7101178e6c). Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkTakumakuComponent;

struct UnkGroundShockerTuning_ovb0023 {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
};
extern UnkGroundShockerTuning_ovb0023 g_unk_71726e85b8;

f32 fn_71005a3cd8(void* self, UnkActor* actor) {
    f32 value = g_unk_71726e85b8.field_0x28;
    UnkTakumakuComponent* anim = static_cast<UnkTakumakuComponent*>(actor->field_0x220);
    if (!anim)
        return value;
    if (fn_7101178e6c(anim, 0xb))
        return g_unk_71726e85b8.field_0x24;
    return value;
}
