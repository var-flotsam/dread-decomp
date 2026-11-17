// UnkTU811 (CEmmyAIComponent's unit): plays the EMMI's sound g_unk_71726e2058 at the entity's position,
// fading out the one started before (the handle at +0x29528). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c2f0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);

__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_71726e2058;

struct UnkEntity_l552af0 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
};

// The handle's view of the reference block.
struct UnkSoundRefView_l552af0 {
    UnkSound* sound;
    s64 serial;
};

static inline UnkSound* Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_l552af0* ref = reinterpret_cast<const UnkSoundRefView_l552af0*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->sound;
    return nullptr;
}

struct UnkEmmyAI_l552af0 {
    u8 pad_0x0[0x30];
    UnkEntity_l552af0* entity;  // 0x30
    u8 pad_0x38[0x29528 - 0x38];
    UnkSoundHandle sound;  // 0x29528
};

void fn_7100552af0(UnkEmmyAI_l552af0* self) {
    UnkInlineSoundDesc desc;
    desc.kind.field_0x0 = 0;
    desc.path = g_unk_71726e2058;
    desc.field_0x6c = false;
    desc.field_0x6d = true;
    UnkEntity_l552af0* entity = self->entity;
    f32 x = entity->transform[0][3];
    f32 y = entity->transform[1][3];
    f32 z = entity->transform[2][3];
    desc.position[0] = x;
    desc.position[1] = y;
    desc.position[2] = z;
    desc.field_0x38 = 0.5f;
    desc.field_0x3c = 1000.0f;
    desc.field_0x40 = 10000.0f;
    desc.field_0x44 = 1.0f;
    desc.field_0x6e = true;
    desc.field_0x5c = 2.0f;
    if (UnkSound* playing = Playing(self->sound)) {
        fn_7100105a80(playing, 0.3f, 0.3f);
        self->sound = g_unk_7108b6c460;
    }
    self->sound = fn_7100d7c2f0(reinterpret_cast<UnkEntityWithSound*>(self->entity),
                                reinterpret_cast<const u32*>(&desc), false, true, true);
}
