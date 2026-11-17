// CPlayerInfo's unit (tools/tu_map.py unit 2592): starts the player info's timed effect once: records
// its position, four values, a second position and a source, fades out any playing vibration and starts
// the unit's looping one (g_unk_71729db988). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CPlayerInfo.h"
#include "game/UnkTU50.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::input::UnkRumbleManager;

struct UnkHandleTarget_ovb0082 {
    void* target;
    u64 serial;
};

struct UnkHandle {
    UnkHandleTarget_ovb0082* target;
    u64 serial;

    void* Get() const {
        if (target && target->serial == serial && serial != ~0ull)
            return target->target;
        return nullptr;
    }
};

struct UnkRumbleParams;
// The unit's looping vibration file.

static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

struct UnkRumbleWords_lf0dac0 {
    u32 x, y, z;
};

// What a rumble is played from (0x48 bytes, UnkRumbleParams of src/game/CRumbleComponent.cpp), with its
// defaults.
struct UnkRumbleParams_lf0dac0 {
    CFilePathStrId path;
    u64 field_0x8;
    s32 field_0x10;
    UnkRumbleWords_lf0dac0 field_0x14;
    u64 field_0x20;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool loop;  // 0x41

    UnkRumbleParams_lf0dac0()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff),
          field_0x14(reinterpret_cast<const UnkRumbleWords_lf0dac0&>(g_unk_710164779c)), field_0x20(0),
          field_0x28(1.0f), field_0x2c(1.0f), field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), loop(false) {}

};

struct UnkPlayerInfoEffect_lf0dac0 {
    u8 pad_0x0[0x104];
    bool active;          // 0x104
    u8 pad_0x105[3];
    f32 time;             // 0x108
    UnkVec3f pos;         // 0x10c
    f32 field_0x118;
    f32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
    UnkVec3f field_0x128;
    u8 pad_0x134[4];
    void* source;         // 0x138
    u8 pad_0x140[0x120];
    UnkHandle rumble;     // 0x260
};
static_assert(__builtin_offsetof(UnkPlayerInfoEffect_lf0dac0, rumble) == 0x260, "layout");

static inline bool IsPlaying(const UnkHandle& h) {
    return h.Get() != g_unk_7101d586c0.Get();
}

void fn_7100f0dac0(UnkPlayerInfoEffect_lf0dac0* self, const UnkVec3f* pos, f32 a, f32 b, f32 c, f32 d,
                   const UnkVec3f* other, void* source) {
    if (self->active)
        return;
    self->active = true;
    self->time = 0.0f;
    self->pos = *pos;
    self->field_0x118 = b;
    self->field_0x11c = c;
    self->field_0x124 = a;
    self->field_0x120 = d;
    self->field_0x128 = *other;
    self->source = source;
    if (IsPlaying(self->rumble))
        fn_7100072328(self->rumble.Get(), 0.2f, 0.0f);
    UnkRumbleParams_lf0dac0 params;
    params.path = g_unk_71729db988;
    params.field_0x10 = 0;
    params.loop = true;
    self->rumble = fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&params));
}
