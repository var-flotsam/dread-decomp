// CGroundShockerAIComponent (unit 861): writing a value mapped from `value` ((value - 184.168) / 200)
// into the property bag of the actor's character class under the id at g_unk_71726e88c8. Names are
// placeholders.
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"
#include "../../game_s7/UnkPropBagS7.h"

struct UnkGroundShockerComponent {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
};

extern char g_unk_71726e88c8[] __attribute__((visibility("hidden")));

void fn_71005a5110(UnkGroundShockerComponent* self, f32 value) {
    UnkAnimationComponent* target = static_cast<UnkAnimationComponent*>(self->field_0x30->field_0x220);
    if (!target)
        return;
    f32 mapped = (value + -184.168f) / 200.0f;
    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(target->field_0x80),
                                *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_71726e88c8), mapped);
}
