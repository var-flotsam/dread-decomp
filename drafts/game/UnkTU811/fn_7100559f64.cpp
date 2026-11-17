// CEmmyAIComponent's unit (tools/tu_map.py unit 811): keeps the Emmy's controller rumble in step with its
// state, starting, retuning or stopping one of four vibration files. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CGrabComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU39.h"
#include "game/UnkTU50.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkUnit50Value_b65a1;
void fn_7100072428(UnkUnit50Value_b65a1* self, bool scaled, f32 target, f32 time);

struct UnkHyperBeamEffectRef;
struct UnkHyperBeamEffectHandle {
    UnkHyperBeamEffectRef* ref;
    s64 serial;
};

// The vibration files.
extern CFilePathStrId g_unk_71726e2060 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_71726e2068 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_71726e2078 __attribute__((visibility("hidden")));
extern CFilePathStrId g_unk_71726e2080 __attribute__((visibility("hidden")));

// A playing rumble: the file it plays at +0x40.
struct UnkRumble_l559f64 {
    u8 pad_0x0[0x40];
    CFilePathStrId path;  // 0x40
};

// The handle's view of the reference block.
struct UnkRumbleRef_l559f64 {
    UnkRumble_l559f64* rumble;
    s64 serial;
};

struct UnkRumbleWords_l559f64 {
    u32 x, y, z;
};

// What a rumble is played from, with its defaults.
struct UnkRumbleDesc_l559f64 {
    CFilePathStrId path;
    u64 field_0x8;
    s32 field_0x10;
    UnkRumbleWords_l559f64 field_0x14;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkRumbleDesc_l559f64()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff),
          field_0x14(reinterpret_cast<const UnkRumbleWords_l559f64&>(g_unk_710164779c)), field_0x20(0.0f),
          field_0x24(0.0f), field_0x28(1.0f), field_0x2c(1.0f), field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};

struct UnkEmmyTunable_l559f64 {
    u8 pad_0x0[0x12a];
    bool enabled;  // 0x12a
    u8 pad_0x12b[0x5];
    u32 rumble_kind;  // 0x130
    u32 state;        // 0x134
};

struct UnkTunableSource_l559f64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkEmmyTunable_l559f64* vf_0xd8();
};

struct UnkEmmyOwner_l559f64 {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
    u8 pad_0x1b5[0x6b];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    void* grab;  // 0x260
};

// The rumble's settings.
struct UnkRumbleState_l559f64 {
    f32 scale;  // 0x0
    u8 pad_0x4[0x20];
    f32 intensity;  // 0x24
    u8 pad_0x28[0x4];
    u32 value;    // 0x2c
    bool field_0x30;
    bool on;  // 0x31
    bool field_0x32;
};

struct UnkEmmyAI_l559f64 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_l559f64* owner;  // 0x30
    u8 pad_0x38[0x14779 - 0x38];
    bool field_0x14779;
    u8 pad_0x1477a[0x14c28 - 0x1477a];
    UnkRumbleState_l559f64 rumble;  // 0x14c28
    u8 pad_0x14c5c[0x29350 - 0x14c5c];
    u8 timer[0x198];  // 0x29350
    UnkHyperBeamEffectHandle other;  // 0x294e8
    u8 pad_0x294f8[0x40];
    UnkHyperBeamEffectHandle handle;  // 0x29538
    u8 pad_0x29548[0x29ee0 - 0x29548];
    UnkTunableSource_l559f64* tunable_source;  // 0x29ee0
};
static_assert(__builtin_offsetof(UnkEmmyAI_l559f64, rumble) == 0x14c28, "layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l559f64, other) == 0x294e8, "layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l559f64, handle) == 0x29538, "layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l559f64, tunable_source) == 0x29ee0, "layout");

static inline UnkRumble_l559f64* Playing(const UnkHyperBeamEffectHandle& handle) {
    const UnkRumbleRef_l559f64* ref = reinterpret_cast<const UnkRumbleRef_l559f64*>(handle.ref);
    if (ref && ref->serial == handle.serial && handle.serial != -1)
        return ref->rumble;
    return nullptr;
}

// Whether the rumble plays `path` (never the empty path).
static inline bool Plays(const UnkRumble_l559f64* rumble, const CFilePathStrId& path) {
    return path.data != &g_unk_7101d47200->empty && rumble->path == path;
}

static inline void Stop(UnkHyperBeamEffectHandle& handle, UnkRumble_l559f64* rumble, f32 time, f32 value) {
    fn_7100072328(rumble, time, value);
    handle = reinterpret_cast<const UnkHyperBeamEffectHandle&>(g_unk_7101d586c0);
}

// Stops the rumble unless it plays `path`.
static inline void StopUnless(UnkHyperBeamEffectHandle& handle, const CFilePathStrId& path) {
    if (UnkRumble_l559f64* rumble = Playing(handle)) {
        if (!Plays(rumble, path))
            Stop(handle, rumble, 0.2f, 0.0f);
    }
}

static inline UnkEmmyTunable_l559f64* Tunable(UnkEmmyAI_l559f64* self) {
    return self->tunable_source->vf_0xd8();
}

// Whether the Emmy is in a state its tunable lets rumble.
static inline bool StateAllows(UnkEmmyAI_l559f64* self) {
    UnkEmmyAIView* view = reinterpret_cast<UnkEmmyAIView*>(self);
    UnkEmmyAI2_b62a3* ai = reinterpret_cast<UnkEmmyAI2_b62a3*>(self);
    UnkEmmyAIComponent_b46a1* component = reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self);
    if (Tunable(self)->state == 0)
        return false;
    if (Tunable(self)->state == 1 && !fn_710054ad2c(view) && !fn_710054aecc(ai))
        return false;
    if (Tunable(self)->state == 2 && !fn_7100543b30(view))
        return false;
    if (Tunable(self)->state == 3 && !fn_71005436e8(component))
        return false;
    if (Tunable(self)->state == 4 && !fn_710054ad2c(view) && !fn_710054aecc(ai) && !fn_7100543b30(view))
        return false;
    if (Tunable(self)->state == 5 && !fn_7100543b30(view) && !fn_71005436e8(component))
        return false;
    if (Tunable(self)->state == 6 && !fn_710054ad2c(view) && !fn_710054aecc(ai) && !fn_71005436e8(component))
        return false;
    return true;
}

// Starts `path`: a pulse fades between 2500 and 500 units; otherwise the rumble follows the intensity.
static inline void Start(UnkEmmyAI_l559f64* self, const CFilePathStrId& path, f32 intensity, bool pulse) {
    UnkRumbleDesc_l559f64 desc;
    desc.field_0x10 = 3;
    desc.path = path;
    desc.field_0x41 = false;
    if (pulse) {
        desc.field_0x20 = 2500.0f;
        desc.field_0x24 = 500.0f;
    } else {
        desc.field_0x20 = 0.0f;
        desc.field_0x24 = 1.0f;
    }
    desc.field_0x28 = intensity * self->rumble.scale;
    desc.field_0x2c = 1.0f;
    desc.field_0x30 = 0.5f;
    desc.field_0x3c = pulse ? 1.0f : intensity + 1.0f;
    desc.field_0x40 = true;
    self->handle = fn_7100d7c614(reinterpret_cast<UnkActor*>(self->owner),
                                 reinterpret_cast<const UnkHyperBeamEffectDesc*>(&desc), &g_unk_7101d470e0, 0, 1, 1);
}

bool fn_7100559f64(UnkEmmyAI_l559f64* self, bool force, u32 value) {
    UnkRumbleState_l559f64* rumble = &self->rumble;
    if (!Tunable(self)->enabled) {
        StopUnless(self->handle, g_unk_7101d470e8);
        return false;
    }
    if (!self->owner->field_0x1b4 || !StateAllows(self)) {
        StopUnless(self->handle, g_unk_7101d470e8);
        return false;
    }
    if (!self->field_0x14779 || fn_710002fb70(self->timer) ||
        (self->owner->grab && fn_710113d83c(self->owner->grab)) ||
        (self->owner->animation && fn_710117b348(self->owner->animation, fn_7100c78d04()))) {
        if (UnkRumble_l559f64* playing = Playing(self->handle)) {
            fn_7100072328(playing, 0.2f, 0.0f);
            return true;
        }
        return false;
    }

    f32 intensity = rumble->intensity;
    switch (Tunable(self)->rumble_kind) {
    case 0:
        if (UnkRumble_l559f64* playing = Playing(self->handle)) {
            if (!Plays(playing, g_unk_71726e2068))
                Stop(self->handle, playing, 0.2f, 0.0f);
            return true;
        }
        Start(self, g_unk_71726e2068, intensity, false);
        return true;
    case 1:
        if (UnkRumble_l559f64* playing = Playing(self->handle)) {
            if (!Plays(playing, g_unk_71726e2060))
                Stop(self->handle, playing, 0.2f, 0.0f);
            return true;
        }
        Start(self, g_unk_71726e2060, intensity, false);
        return true;
    case 2: {
        StopUnless(self->handle, g_unk_71726e2078);
        bool keep;
        u32 state = Tunable(self)->state;
        if ((state == 7 || Tunable(self)->state == 3 || Tunable(self)->state == 5 || Tunable(self)->state == 6) &&
            fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self))) {
            rumble->on = false;
            keep = true;
        } else if ((force || rumble->value == value) && rumble->field_0x32) {
            UnkRumble_l559f64* other = Playing(self->other);
            if (UnkRumble_l559f64* playing = Playing(self->handle)) {
                if (!rumble->on)
                    Stop(self->handle, playing, 0.0f, 0.0f);
            }
            keep = other != nullptr;
        } else {
            keep = false;
        }
        if (keep) {
            if (rumble->on || Playing(self->handle))
                return true;
            rumble->on = true;
            Start(self, g_unk_71726e2078, intensity, true);
            return true;
        }
        if (!rumble->field_0x30 && rumble->on) {
            if (UnkRumble_l559f64* playing = Playing(self->handle))
                Stop(self->handle, playing, 0.2f, 0.0f);
            rumble->on = false;
        }
        return false;
    }
    case 3:
        StopUnless(self->handle, g_unk_71726e2080);
        if (UnkRumble_l559f64* playing = Playing(self->handle)) {
            fn_7100072428(reinterpret_cast<UnkUnit50Value_b65a1*>(playing), false, intensity * rumble->scale, 0.2f);
            return true;
        }
        Start(self, g_unk_71726e2080, intensity, false);
        return true;
    }
    return false;
}
