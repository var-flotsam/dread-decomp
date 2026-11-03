// CShipRechargeComponent unit (tools/tu_map.py): when the sound handle at +0x540 still names a live
// sound, has the owner entity (+0x30) stop it (fn_7100d7c510, 0.25 fade) and empties the handle.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSound;
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;

    UnkSound* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkShipRechargeEntity_b49a4;
void fn_7100d7c510(UnkShipRechargeEntity_b49a4* entity, UnkSoundHandle* handle, f32 fade, f32 delay);

struct UnkShipRecharge_b49a4 {
    u8 pad_0x0[0x30];
    UnkShipRechargeEntity_b49a4* entity;
    u8 pad_0x38[0x508];
    UnkSoundHandle sound;  // +0x540
};

void fn_7100a8bf18(UnkShipRecharge_b49a4* self) {
    if (self->sound.Get() == nullptr)
        return;
    fn_7100d7c510(self->entity, &self->sound, 0.25f, 0.0f);
    self->sound = g_unk_7108b6c460;
}
