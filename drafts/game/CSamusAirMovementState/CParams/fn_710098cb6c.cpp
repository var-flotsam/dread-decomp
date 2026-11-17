// CSamusAirMovementState::CParams' unit (tools/tu_map.py): plays the spider ball's oil-out sound on the owner
// (+0x30) from a sound description kept in a static, its path picked at random. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2772.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The sound description the entity plays, with its defaults.
struct UnkSoundDesc_l98cb6c {
    u32 kind;
    u32 field_0x4;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    CStrId name;          // 0x18
    CFilePathStrId path;  // 0x20
    f32 position[3];      // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkSoundDesc_l98cb6c()
        : kind(0), field_0x4(0), field_0x8(0), field_0x10(0), name(g_unk_7101d470e0), path(g_unk_7101d470e8),
          position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f), field_0x3c(500.0f), field_0x40(2000.0f),
          field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f), field_0x50(0.0f), field_0x54(-1.0f),
          field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f), field_0x64(5.0f), field_0x68(10.0f),
          field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundDesc_l98cb6c() asm("fn_710034e0b8");
};
static_assert(__builtin_offsetof(UnkSoundDesc_l98cb6c, field_0x6c) == 0x6c, "layout");

struct UnkAirParams_l98cb6c {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;  // 0x30
};

UnkSoundHandle fn_710098cb6c(UnkAirParams_l98cb6c* self) {
    static const CFilePathStrId s_oil_out_1("actors/morphball/spiderball_oil_out_01.wav", true);
    static const CFilePathStrId s_oil_out_2("actors/morphball/spiderball_oil_out_02.wav", true);
    static UnkSoundDesc_l98cb6c s_desc;
    s_desc.field_0x6d = true;
    s_desc.kind = 0;
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
