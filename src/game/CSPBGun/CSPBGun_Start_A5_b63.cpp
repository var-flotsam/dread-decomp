// CSPBGun's unit (tools/tu_map.py): starts once (+0x1880) when fn_710085e6d4 allows it: clears +0x1884 and
// +0x18e8, sets a fixed id on the owner's timeline (+0x18, +0x2e8) to the all-flags mask, keeps whether
// fn_710085e7e8 said no at +0x18e8, and, unless the sound handle at +0x1890 is still live, plays the sound
// +0x1874 at the owner's position (fn_7100ed6910, as src/game/CGrappleBeamGun/CGrappleBeamGun.cpp declares
// it) and keeps the new handle. Returns whether it started. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::math::CVector3D;

struct UnkSoundObject_A5_b63 {
    u8 pad_0x0[0x122];
    bool field_0x122;
};

struct UnkHyperBeamSoundRef {
    UnkSoundObject_A5_b63* object;
    s64 serial;
};

struct UnkHyperBeamSoundHandle {
    UnkHyperBeamSoundRef* ref;
    s64 serial;
};

struct UnkSPBGun_A5_b63;
bool fn_710085e6d4(UnkSPBGun_A5_b63* self);
bool fn_710085e7e8(UnkSPBGun_A5_b63* self);

struct UnkSPBPosition_A5_b63 {
    f32 x, y, z;
};

struct UnkSPBGun_A5_b63 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x1874 - 0x20];
    u32 sound;  // 0x1874
    u8 pad_0x1878[0x8];
    bool started;  // 0x1880
    u8 pad_0x1881[3];
    u32 field_0x1884;
    u32 field_0x1888;
    u8 pad_0x188c[4];
    UnkHyperBeamSoundHandle handle;  // 0x1890
    u8 pad_0x18a0[0x18e8 - 0x18a0];
    bool field_0x18e8;
};

bool fn_710085e504(UnkSPBGun_A5_b63* self) {
    if (self->started || !fn_710085e6d4(self))
        return false;
    self->started = true;
    self->field_0x1884 = 0;
    self->field_0x1888 = 0;
    self->field_0x18e8 = false;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        u64 key = 0xef56c664609e1641ull;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
    }
    self->field_0x18e8 = !fn_710085e7e8(self);
    UnkHyperBeamSoundRef* ref = self->handle.ref;
    if (!ref || ref->serial != self->handle.serial || self->handle.serial == -1 || !ref->object ||
        ref->object->field_0x122) {
        void* player = g_unk_71726af0b0;
        UnkActor* actor = self->owner;
        f32 x = actor->field_0x44;
        f32 y = actor->field_0x54;
        f32 z = actor->field_0x64;
        UnkSPBPosition_A5_b63 pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        self->handle = fn_7100ed6910(player, reinterpret_cast<const CVector3D*>(&pos), self->sound, 3, false);
    }
    return true;
}
