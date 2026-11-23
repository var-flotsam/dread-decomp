// CPlayerInfo's unit (tools/tu_map.py unit 2592): turns the player info's looping vibration on or off:
// on starts the unit's vibration file (g_unk_71729db988), off fades the playing one out.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CPlayerInfo.h"
#include "game/UnkHandleTarget_ovb0082.h"
#include "game/UnkTU50.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::input::UnkRumbleManager;

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

struct UnkRumbleWords_lf0e238 {
    u32 x, y, z;
};

// What a rumble is played from (0x48 bytes, UnkRumbleParams of src/game/CRumbleComponent.cpp), with its
// defaults.
struct UnkRumbleParams_lf0e238 {
    CFilePathStrId path;
    u64 field_0x8;
    s32 field_0x10;
    UnkRumbleWords_lf0e238 field_0x14;
    u64 field_0x20;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool loop;  // 0x41

    UnkRumbleParams_lf0e238()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff),
          field_0x14(reinterpret_cast<const UnkRumbleWords_lf0e238&>(g_unk_710164779c)), field_0x20(0),
          field_0x28(1.0f), field_0x2c(1.0f), field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), loop(false) {}

};

struct UnkPlayerInfoRumble_lf0e238 {
    u8 pad_0x0[0xf4];
    bool rumbling;     // 0xf4
    u8 pad_0xf5[3];
    f32 field_0xf8;
    u8 pad_0xfc[4];
    u32 field_0x100;
    u8 pad_0x104[0x15c];
    UnkHandle rumble;  // 0x260
};
static_assert(__builtin_offsetof(UnkPlayerInfoRumble_lf0e238, rumble) == 0x260, "layout");

static inline bool IsPlaying(const UnkHandle& h) {
    return h.Get() != g_unk_7101d586c0.Get();
}

void fn_7100f0e238(void* target, bool on) {
    UnkPlayerInfoRumble_lf0e238* self = static_cast<UnkPlayerInfoRumble_lf0e238*>(target);
    if (self->rumbling == on)
        return;
    self->field_0xf8 = 0.0f;
    self->rumbling = on;
    if (on) {
        UnkRumbleParams_lf0e238 params;
        params.path = g_unk_71729db988;
        params.field_0x10 = 0;
        params.loop = true;
        self->rumble = fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&params));
    } else {
        if (IsPlaying(self->rumble))
            fn_7100072328(self->rumble.Get(), 0.2f, 0.0f);
        self->field_0x100 = 0;
    }
}
