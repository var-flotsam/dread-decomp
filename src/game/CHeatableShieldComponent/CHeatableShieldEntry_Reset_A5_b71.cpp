// CHeatableShieldComponent unit (unit 1323): resets one 0xd0-byte shield entry. It clears its counters, then for
// each of its two sound handles (+0xa8, +0xb8) that still names a live sound has the owner entity (+0xc8) stop it
// (fn_7100d7c510, no fade) and empties the handle. Names and layouts are placeholders.
#include "game/UnkTU2523.h"
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

// Signature as src/game/CShipRechargeComponent/CShipRechargeComponent_StopSound_b49a4.cpp declares it.
struct UnkShipRechargeEntity_b49a4;

// The entry type src/game/CHeatableShieldComponent/CHeatableShieldComponent_2.cpp passes.
struct UnkHeatableShieldEntry;

struct UnkHeatableShieldEntry_A5_b71 {
    u8 pad_0x0[0x50];
    u64 field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    u8 pad_0x64[0x3c];
    u16 field_0xa0;
    u8 pad_0xa2[0x6];
    UnkSoundHandle sound_a;  // 0xa8
    UnkSoundHandle sound_b;  // 0xb8
    UnkShipRechargeEntity_b49a4* entity;  // 0xc8
};
static_assert(__builtin_offsetof(UnkHeatableShieldEntry_A5_b71, entity) == 0xc8, "layout");

static inline __attribute__((always_inline)) void Stop_A5_b71(UnkHeatableShieldEntry_A5_b71* self,
                                                              UnkSoundHandle* h) {
    if (h->Get() == nullptr)
        return;
    fn_7100d7c510(self->entity, h, 0.0f, 0.0f);
    *h = g_unk_7108b6c460;
}

void fn_710086e410(UnkHeatableShieldEntry* entry) {
    UnkHeatableShieldEntry_A5_b71* self = reinterpret_cast<UnkHeatableShieldEntry_A5_b71*>(entry);
    self->field_0xa0 = 0;
    self->field_0x60 = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    Stop_A5_b71(self, &self->sound_a);
    Stop_A5_b71(self, &self->sound_b);
}
