// CHeatRoomComponent unit (tools/tu_map.py unit 1538): on entering the heat room, unless it is
// disabled, plays "ThermalRoomEnter" once and notes whether the environment knows the room's preset,
// then runs the damage trigger's step (fn_7100a45cf0). Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c3b0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);

struct UnkDamageTrigger_b0119;
// The definition (CCharClassBaseDamageTriggerComponent_Update_ovb0119.cpp) has two parameters; the
// original passes a third here.
void fn_7100a45cf0(UnkDamageTrigger_b0119* self, void* arg, void* arg2);

void* fn_7100d8efc0(void* manager, const CStrId* id);

// The handle's view of the sound manager's reference block.
struct UnkSoundRefView_la27414 {
    void* sound;
    s64 serial;
};

static inline void* Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_la27414* ref = reinterpret_cast<const UnkSoundRefView_la27414*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->sound;
    return nullptr;
}

// The environment manager, built on first use.
static inline void* EnvironmentManager() {
    if (!g_unk_7101d47368) {
        void* manager = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    return g_unk_7101d47368;
}

struct UnkHeatRoom_la27414 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;  // 0x30
    u8 pad_0x38[0x9b8 - 0x38];
    u32 disabled;  // 0x9b8
    u8 pad_0x9bc[0xc50 - 0x9bc];
    CFilePathStrId enter_sound;  // 0xc50
    CStrId preset;               // 0xc58
    u8 pad_0xc60[0x8];
    bool has_preset;     // 0xc68
    UnkSoundHandle sound;  // 0xc70
};
static_assert(__builtin_offsetof(UnkHeatRoom_la27414, sound) == 0xc70, "layout");

void fn_7100a27414(UnkHeatRoom_la27414* self, void* arg, void* arg2) {
    if (fn_7100df5c38(g_unk_7101d47310, arg) && self->disabled == 0) {
        if (!Playing(self->sound)) {
            static UnkSoundDesc s_sound;
            s_sound.field_0x8.field_0x0 = 0;
            s_sound.name = CStrId("ThermalRoomEnter", true);
            s_sound.path = self->enter_sound;
            s_sound.field_0x38 = 0.4f;
            self->sound = fn_7100d7c3b0(self->entity, reinterpret_cast<const u32*>(&s_sound), false, true, true);
        }
        self->has_preset = self->preset.data != &g_unk_7101d47108->empty &&
                           fn_7100d8efc0(EnvironmentManager(), &self->preset) != nullptr;
    }
    fn_7100a45cf0(reinterpret_cast<UnkDamageTrigger_b0119*>(self), arg, arg2);
}
