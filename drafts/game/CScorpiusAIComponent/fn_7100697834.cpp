// CScorpiusAIComponent unit: as the range update next to it (the horizontal range at +0x14cf0 from the first
// and last path point, widened by two tunables), then hands the range out through two pointers; always true.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CCharClassScorpiusAIComponent.h"
#include "game/UnkTUProperty.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkXformNode.h"
#include "types.h"


struct UnkScorpiusTunables_l697834 {
    u8 pad_0x0[0x4c];
    f32 field_0x4c;
    f32 field_0x50;
};

struct UnkScorpiusOwner_l697834 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
};

struct UnkScorpiusAI_l697834 {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner_l697834* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkVec3f* points;  // 0x68
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 flag;  // 0x94
    u8 pad_0x95[0x14cf0 - 0x95];
    f32 range[2];  // 0x14cf0
};

static inline const UnkScorpiusTunables_l697834* Tunables() {
    return reinterpret_cast<const UnkScorpiusTunables_l697834*>(&g_unk_7172703e88);
}

bool fn_7100697834(UnkScorpiusAI_l697834* self, u32* outStart, u32* outEnd) {
    u32 n = self->count;
    u32 last = n - ((self->flag == 0) & (n != 0));
    f32* out = self->range;
    if (last == 0) {
        *outStart = __builtin_bit_cast(u32, out[0]);
    } else {
        UnkVec3f end = self->points[last];
        UnkVec3f start = self->points[0];
        if (fn_710002614c(&self->owner->xform).m[2] > 0.0f) {
            out[0] = start.x + Tunables()->field_0x50;
            out[1] = end.x - Tunables()->field_0x4c;
        } else {
            out[0] = end.x - Tunables()->field_0x50;
            out[1] = start.x + Tunables()->field_0x4c;
        }
        *outStart = __builtin_bit_cast(u32, out[0]);
    }
    *outEnd = __builtin_bit_cast(u32, out[1]);
    return true;
}
