// UnkTU1376 (tools/tu_map.py, unit 1376; near CDoorLifeComponent): when either of the two states at +0x654
// and +0x658 is 1, plays the unit's door sound on the owner from a shared description.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

// The unit's sound path (set by the unit's static initialiser).
static CFilePathStrId g_unk_7172731730;

struct UnkDoorLife_l8d07f8 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;  // 0x30
    u8 pad_0x38[0x654 - 0x38];
    s32 field_0x654;
    s32 field_0x658;
};

void fn_71008d07f8(UnkDoorLife_l8d07f8* self) {
    if (self->field_0x654 != 1 && self->field_0x658 != 1)
        return;
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.path = g_unk_7172731730;
    s_desc.field_0x38 = 1.0f;
    s_desc.field_0x3c = 300.0f;
    s_desc.field_0x40 = 1000.0f;
    s_desc.field_0x44 = 1.4f;
    s_desc.field_0x6c = true;
    s_desc.field_0x6d = true;
    fn_7100d7c374(self->owner, &s_desc, &g_unk_7101d470e0, false, true, true);
}
