// CShakernautAIComponent unit (tools/tu_map.py unit 1047): takes one of the two ranges (first or second)
// from the enemy movement's parameters (when its mode is 1) and marks it taken; then lets the playing
// sound fade on (fn_71001059f0) or starts the sound g_unk_7172706498 with a shared request.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/UnkSound.h"
#include "game/CEnemyMovement.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

struct UnkInterp_7100105d34;
void fn_71001059f0(UnkInterp_7100105d34* self, f32 time);

extern CFilePathStrId g_unk_7172706498 __attribute__((visibility("hidden")));

// A sound request (what fn_7100d7c374 reads), with its defaults.
struct UnkSoundRequest_l6b1228 {
    u32 field_0x0;
    f32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    CStrId id;             // 0x18
    CFilePathStrId path;   // 0x20
    f32 pos[3];            // 0x28
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

    UnkSoundRequest_l6b1228()
        : field_0x0(0), field_0x4(0.0f), field_0x8(0), field_0xc(0), field_0x10(0), id(g_unk_7101d470e0),
          path(g_unk_7101d470e8), pos{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f),
          field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f),
          field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f),
          field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
};
static_assert(__builtin_offsetof(UnkSoundRequest_l6b1228, field_0x6e) == 0x6e, "layout");

struct UnkRange_l6b1228 {
    f32 min;
    f32 max;

    UnkRange_l6b1228& operator=(const UnkRange_l6b1228& other) {
        min = other.min;
        max = other.max;
        return *this;
    }
};

struct UnkMovementParams_l6b1228 {
    u8 pad_0x0[0x8c];
    UnkRange_l6b1228 range;  // 0x8c
};

struct UnkMovement_l6b1228 {
    void** vtable;
    u8 pad_0x8[0x8e0];
    UnkMovementParams_l6b1228 params;  // 0x8e8

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkMovement_l6b1228*)>(vtable[0])(this);
    }
};

struct UnkActor_l6b1228 {
    u8 pad_0x0[0x1f0];
    UnkMovement_l6b1228* movement;  // 0x1f0
};

// The handle's view of the reference block.
struct UnkSoundRefView_l6b1228 {
    UnkSound* sound;
    s64 serial;
};

struct UnkShakernautSound_l6b1228 {
    bool taken_first;   // 0x14868
    bool taken_second;  // 0x14869
    u8 pad_0x2[0x12];
    UnkRange_l6b1228 first;   // 0x1487c
    UnkRange_l6b1228 second;  // 0x14884
    u8 pad_0x24[4];
    UnkSoundHandle handle;  // 0x14890

    UnkSound* Playing() const {
        const UnkSoundRefView_l6b1228* ref = reinterpret_cast<const UnkSoundRefView_l6b1228*>(handle.ref);
        if (ref && ref->serial == handle.serial && handle.serial != -1)
            return ref->sound;
        return nullptr;
    }
};

struct UnkShakernautAI_l6b1228 {
    u8 pad_0x0[0x30];
    UnkActor_l6b1228* actor;  // 0x30
    u8 pad_0x38[0x14868 - 0x38];
    UnkShakernautSound_l6b1228 sound;  // 0x14868
};
static_assert(__builtin_offsetof(UnkShakernautAI_l6b1228, sound) == 0x14868, "layout");

void fn_71006b1228(UnkShakernautAI_l6b1228* self, bool first) {
    UnkShakernautSound_l6b1228* sound = &self->sound;
    UnkMovement_l6b1228* movement = self->actor->movement;
    movement = movement && fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) ? movement : nullptr;
    UnkMovementParams_l6b1228* params = fn_71010cc3a4(movement) == 1 ? &movement->params : nullptr;
    if (first) {
        sound->first = params->range;
        sound->taken_first = true;
    } else {
        sound->second = params->range;
        sound->taken_second = true;
    }
    if (UnkSound* playing = sound->Playing()) {
        if (!playing->field_0x118)
            fn_71001059f0(reinterpret_cast<UnkInterp_7100105d34*>(playing), 0.2f);
        return;
    }
    static UnkSoundRequest_l6b1228 request;
    request.field_0x0 = 0;
    request.path = g_unk_7172706498;
    request.field_0x38 = 1.0f;
    request.field_0x4c = 0.0f;
    request.field_0x6c = true;
    request.field_0x6d = true;
    sound->handle = fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->actor), &request, &g_unk_7101d470e0,
                                  false, true, true);
}
