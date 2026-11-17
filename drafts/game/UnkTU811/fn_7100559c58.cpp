// CEmmyAIComponent's unit (tools/tu_map.py unit 811): keeps the Emmy's sound (+0x29518) in step with the
// state's settings. Stops it when the settings say none should play, or the owner's value at +0x1f0 has
// settled at zero; otherwise, while nothing plays, fills the shared sound description from the settings.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_71726e2038;

// The handle's view of the reference block.
struct UnkSoundRefView_l559c58 {
    void* sound;
    s64 serial;
};

static inline bool Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_l559c58* ref = reinterpret_cast<const UnkSoundRefView_l559c58*>(handle.ref);
    return ref && ref->serial == handle.serial && handle.serial != -1 && ref->sound;
}

struct UnkEmmySoundSettings_l559c58 {
    u8 pad_0x0[0xd0];
    f32 volume;    // 0xd0
    f32 minDist;   // 0xd4
    f32 maxDist;   // 0xd8
    u8 pad_0xdc[0x1b0 - 0xdc];
    bool tracked;  // 0x1b0
};

struct UnkEmmySettingsSource_l559c58 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkEmmySoundSettings_l559c58* GetSettings();  // 0xd8
};

struct UnkEmmyMotion_l559c58 {
    u8 pad_0x0[0x118];
    f32 value;  // 0x118
};

struct UnkEmmyEntity_l559c58 {
    u8 pad_0x0[0x1f0];
    UnkEmmyMotion_l559c58* motion;  // 0x1f0
};

struct UnkEmmyStateAI_l559c58 {
    u8 pad_0x0[0x30];
    UnkEmmyEntity_l559c58* entity;  // 0x30
    u8 pad_0x38[0x29518 - 0x38];
    UnkSoundHandle sound;           // 0x29518
    u8 pad_0x29528[0x29ee0 - 0x29528];
    UnkEmmySettingsSource_l559c58* settings;  // 0x29ee0
};

static inline bool NearZero(f32 value) {
    return value - 0.001f <= 0.0f && value + 0.001f >= 0.0f;
}

static inline void StopSound(UnkEmmyStateAI_l559c58* self) {
    fn_7100d7c510(reinterpret_cast<UnkShipRechargeEntity_b49a4*>(self->entity), &self->sound, 0.0f, 0.0f);
    self->sound = g_unk_7108b6c460;
}

void fn_7100559c58(UnkEmmyStateAI_l559c58* self) {
    if (!self->settings->GetSettings()->tracked) {
        if (Playing(self->sound))
            StopSound(self);
        return;
    }
    UnkEmmyMotion_l559c58* motion = self->entity->motion;
    f32 value = motion ? motion->value : 0.0f;
    bool settled = NearZero(value);
    bool playing = Playing(self->sound);
    if (settled) {
        if (playing)
            StopSound(self);
        return;
    }
    if (playing)
        return;
    static UnkSoundDesc s_desc;
    s_desc.path = g_unk_71726e2038;
    s_desc.kind.field_0x0 = 6;
    s_desc.field_0x6d = true;
    s_desc.field_0x38 = self->settings->GetSettings()->volume;
    s_desc.field_0x3c = self->settings->GetSettings()->minDist;
    s_desc.field_0x40 = self->settings->GetSettings()->maxDist;
    s_desc.field_0x6c = true;
}
