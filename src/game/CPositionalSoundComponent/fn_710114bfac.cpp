// CPositionalSoundComponent unit: stores a new volume and passes it (at least 0.001) with a fade time to both
// playing sound handles. Names and layouts are placeholders.
#include "base/snd/UnkSound.h"
#include "game/UnkSoundRef.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

static inline UnkSound* GetSound_l14bfac(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

struct UnkPositionalSound_l14bfac {
    u8 unk_0x0[0x7c];
    f32 volume;  // 0x7c
    u8 unk_0x80[8];
    UnkSoundHandle handles[2];  // 0x88
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710114bfac(UnkPositionalSound_l14bfac* self, f32 volume, f32 time) {
    self->volume = volume;
    for (const UnkSoundHandle& handle : self->handles) {
        if (UnkSound* sound = GetSound_l14bfac(&handle)) {
            f32 clamped = self->volume > 0.001f ? self->volume : 0.001f;
            fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(sound), clamped, false, time);
        }
    }
}
