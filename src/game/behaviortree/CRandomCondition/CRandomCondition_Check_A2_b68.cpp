// behaviortree::CRandomCondition's unit (tools/tu_map.py, the name from reflection): holds with the probability at
// +0x38, which a set blackboard property (+0x40, a string id) overrides through fn_7100394e2c; a random value in
// 0..1 is compared with it. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2772.h"
#include "types.h"

// Signature as src/game/behaviortree/CStopWatchAction/fn_7100392704.cpp declares it.
struct UnkSelf;
void fn_7100394e2c(UnkSelf* self, void* record, f32* out);

struct UnkRandomCondition_A2_b68 {
    u8 pad_0x0[0x38];
    f32 probability;
    u8 pad_0x3c[0x4];
    base::global::CStrId property;
};

bool fn_710039f194(UnkRandomCondition_A2_b68* self) {
    f32 probability = self->probability;
    if (self->property.data != &g_unk_7101d47108->empty)
        fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), &self->property, &probability);
    f32 lo = 0.0f;
    f32 hi = 1.0f;
    f32 r = (f32)((fn_71012395a0() & 0x7fff) * (1.0 / 32767.0));
    return lo + (hi - lo) * r <= probability;
}
