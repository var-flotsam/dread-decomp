// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009aefb8 starts the looping vibration (the unit's
// file at g_unk_717273c6f0) unless the rumble the handle at +0x338 refers to is still playing and not stopping
// (+0x9d). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
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

extern UnkHandle g_unk_7101d586c0;
extern UnkRumbleManager* g_unk_7101d47348;
struct UnkRumbleParams;
UnkHandle fn_7100074354(UnkRumbleManager* self, const UnkRumbleParams* params);
// The state's looping vibration file (in the unit's data, src/game/CSamusMagnetGloveMovementState/
// CSamusMagnetGloveMovementState_TUInit.cpp).
extern CFilePathStrId g_unk_717273c6f0 __attribute__((visibility("hidden")));

static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

struct UnkRumbleWords_ovb0082 {
    u32 x, y, z;
};

// What a rumble is played from (0x48 bytes, UnkRumbleParams of src/game/CRumbleComponent.cpp), with its
// defaults.
struct UnkRumbleParams_ovb0082 {
    CFilePathStrId path;
    u64 field_0x8;
    s32 field_0x10;
    UnkRumbleWords_ovb0082 field_0x14;
    u64 field_0x20;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    u16 field_0x40;

    UnkRumbleParams_ovb0082()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff),
          field_0x14(reinterpret_cast<const UnkRumbleWords_ovb0082&>(g_unk_710164779c)), field_0x20(0),
          field_0x28(1.0f), field_0x2c(1.0f), field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(0) {}

    void SetPath(const CFilePathStrId& other) {
        UnkStringData* old = path.data;
        UnkStringData* d = other.data;
        if (d != old) {
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (old != &pool->empty) {
                if (--old->ref_count == 0)
                    fn_71000803a4(pool, old);
            }
            path.data = d;
        }
    }
};

struct UnkMagnetRumble_ovb0082 {
    u8 pad_0x0[0x9d];
    bool field_0x9d;
};

struct UnkMagnetLoopState_ovb0082 {
    u8 pad_0x0[0x338];
    UnkHandle rumble;  // 0x338
};

static inline bool IsPlaying(const UnkHandle& h) {
    return h.Get() != g_unk_7101d586c0.Get();
}

void fn_71009aefb8(UnkMagnetLoopState_ovb0082* self) {
    if (IsPlaying(self->rumble) &&
        (!IsPlaying(self->rumble) || static_cast<UnkMagnetRumble_ovb0082*>(self->rumble.target->target)->field_0x9d))
        return;
    UnkRumbleParams_ovb0082 params;
    params.SetPath(g_unk_717273c6f0);
    params.field_0x28 = 0.3f;
    params.field_0x10 = 0;
    self->rumble = fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&params));
}
