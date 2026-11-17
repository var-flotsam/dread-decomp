// CHeatableShieldComponent unit (unit 1322): unless the entry's sound (+0xb8) still plays, has the owner
// entity (+0x30) play the shield's sound path (g_unk_7172729398, a unit static) with fixed parameters and keeps the handle.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

// The shield sound path, set up by the unit's static initialiser.
static CFilePathStrId s_shield_sound_path;

struct UnkSound;
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};

// The parameters a sound is played with.
struct UnkShieldSoundParams_l86bd4c {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u64 field_0x8 = 0;
    u32 field_0x10 = 0;
    CStrId id = g_unk_7101d470e0;               // 0x18
    CFilePathStrId path = g_unk_7101d470e8;     // 0x20
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
};
static_assert(sizeof(UnkShieldSoundParams_l86bd4c) == 0x70, "layout");

struct UnkHeatableShield_l86bd4c {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* owner;  // 0x30
};

struct UnkHeatableShieldEntry_l86bd4c {
    u8 pad_0x0[0xb8];
    UnkSoundHandle sound;  // 0xb8
};

static inline bool Playing(const UnkSoundHandle& h) {
    return h.ref && h.ref->serial == h.serial && h.serial != -1 && h.ref->sound;
}

void fn_710086bd4c(UnkHeatableShield_l86bd4c* self, UnkHeatableShieldEntry_l86bd4c* entry) {
    if (Playing(entry->sound))
        return;
    static UnkShieldSoundParams_l86bd4c s_params;
    s_params.field_0x0 = 0;
    s_params.path = s_shield_sound_path;
    s_params.field_0x38 = 1.0f;
    s_params.field_0x3c = 500.0f;
    s_params.field_0x40 = 1500.0f;
    s_params.field_0x44 = 1.4f;
    s_params.field_0x6c = false;
    s_params.field_0x6d = true;
    entry->sound = fn_7100d7c374(self->owner, &s_params, &g_unk_7101d470e0, false, true, true);
}
