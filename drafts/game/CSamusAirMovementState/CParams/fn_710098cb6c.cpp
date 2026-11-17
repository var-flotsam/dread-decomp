// CSamusAirMovementState::CParams' unit (tools/tu_map.py): plays the spider ball's oil-out sound on the owner
// (+0x30) from a sound description kept in a static, its path picked at random. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2772.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

struct UnkAirParams_l98cb6c {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;  // 0x30
};

UnkSoundHandle fn_710098cb6c(UnkAirParams_l98cb6c* self) {
    static const CFilePathStrId s_oil_out_1("actors/morphball/spiderball_oil_out_01.wav", true);
    static const CFilePathStrId s_oil_out_2("actors/morphball/spiderball_oil_out_02.wav", true);
    static UnkSoundDesc s_desc;
    s_desc.field_0x6d = true;
    s_desc.kind.field_0x0 = 0;
    s_desc.field_0x38 = 0.65f;
    s_desc.field_0x3c = 500.0f;
    s_desc.field_0x40 = 50000.0f;
    s_desc.field_0x44 = 1.3f;
    // Both draws pick the first sound in the original.
    switch (fn_71012395a0() & 1) {
    case 0:
        s_desc.path = s_oil_out_1;
        break;
    case 1:
        s_desc.path = s_oil_out_1;
        break;
    }
    return fn_7100d7c374(self->owner, &s_desc, &g_unk_7101d470e0, false, true, true);
}
