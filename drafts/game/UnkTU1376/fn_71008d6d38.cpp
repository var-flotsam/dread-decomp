// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): when either of the two states at
// +0x654/+0x658 is 1, plays the door's sound (path g_unk_7172731728) through the owner entity with a sound
// description kept in a function-local static. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The unit's own path id (set up by the unit's static initialiser).
static CFilePathStrId g_unk_7172731728;

UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

struct UnkDoorLife_l8d6d38 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;
    u8 pad_0x38[0x654 - 0x38];
    s32 field_0x654;
    s32 field_0x658;
};

void fn_71008d6d38(UnkDoorLife_l8d6d38* self) {
    if (self->field_0x654 != 1 && self->field_0x658 != 1)
        return;
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.path = g_unk_7172731728;
    s_desc.field_0x38 = 1.0f;
    s_desc.field_0x3c = 500.0f;
    s_desc.field_0x40 = 1400.0f;
    s_desc.field_0x44 = 1.4f;
    s_desc.field_0x6c = true;
    s_desc.field_0x6d = true;
    fn_7100d7c374(self->entity, &s_desc, &g_unk_7101d470e0, false, true, true);
}
