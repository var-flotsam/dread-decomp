// UnkTU1376 (tools/tu_map.py, unit 1376; near CDoorLifeComponent): when either of the two states at +0x654
// and +0x658 is 1, plays the door's sound (the path g_unk_7172731730) on the owner with a shared parameter
// block, set up once and updated on every play. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The unit's own path id (set by the unit's static initialiser).
static CFilePathStrId g_unk_7172731730;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The parameters of a sound play: a name and a path, then levels and ranges.
struct UnkDoorSoundParams_l8d07f8 {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u64 field_0x8 = 0;
    u32 field_0x10 = 0;
    CStrId id = g_unk_7101d470e0;              // 0x18
    CFilePathStrId path = g_unk_7101d470e8;    // 0x20
    u64 field_0x28 = 0;
    f32 field_0x30 = 0.0f;
    f32 field_0x34 = -1.0f;
    f32 field_0x38 = 1.0f;
    f32 field_0x3c = 500.0f;
    f32 field_0x40 = 2000.0f;
    f32 field_0x44 = 1.0f;
    u64 field_0x48 = 0;
    f32 field_0x50 = 0.0f;
    f32 field_0x54 = -1.0f;
    f32 field_0x58 = -1.0f;
    f32 field_0x5c = -1.0f;
    f32 field_0x60 = 0.5f;
    f32 field_0x64 = 5.0f;
    f32 field_0x68 = 10.0f;
    bool field_0x6c = false;
    bool field_0x6d = false;
    bool field_0x6e = false;

    ~UnkDoorSoundParams_l8d07f8() asm("fn_710034e0b8");
};
static_assert(sizeof(UnkDoorSoundParams_l8d07f8) == 0x70, "params layout");

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
    static UnkDoorSoundParams_l8d07f8 s_params;
    s_params.field_0x0 = 0;
    s_params.path = g_unk_7172731730;
    s_params.field_0x38 = 1.0f;
    s_params.field_0x3c = 300.0f;
    s_params.field_0x40 = 1000.0f;
    s_params.field_0x44 = 1.4f;
    s_params.field_0x6c = true;
    s_params.field_0x6d = true;
    fn_7100d7c374(self->owner, &s_params, &g_unk_7101d470e0, false, true, true);
}
