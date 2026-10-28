// From unit 46 of tools/tu_map.py (0x710006d2a0-0x710006f390, no class hint): the controllers'
// part of the input system (each pad's Npad id, style and vibration devices). Names and layouts are
// placeholders.
// Holds fn_710006ddc8 (sets up the vibration devices) and fn_710006deac (the per-frame update).
#include <nn/hid.h>

#include "base/input/UnkHidVibration.h"
#include "types.h"

#ifdef NNSDK
// The matching build's SDK headers have the Npad types but not the vibration devices; the port's
// runtime declares all of these (port/runtime/include/nn/hid.h).
namespace nn::hid {
using NpadIdType = u32;
s32 GetVibrationDeviceHandles(VibrationDeviceHandle* out, s32 max, const NpadIdType& id, NpadStyleSet styles);
void InitializeVibrationDevice(const VibrationDeviceHandle& handle);
void GetVibrationDeviceInfo(VibrationDeviceInfo* out, const VibrationDeviceHandle& handle);
void SetNpadJoyHoldType(NpadJoyHoldType type);
}  // namespace nn::hid
// The SDK headers give ControllerSupportArg no members: its SetDefault, by its symbol.
void UnkControllerSupportArgSetDefault(nn::hid::ControllerSupportArg* self)
    asm("_ZN2nn3hid20ControllerSupportArg10SetDefaultEv");
#else
static inline void UnkControllerSupportArgSetDefault(nn::hid::ControllerSupportArg* self) { self->SetDefault(); }
#endif

// The pads (up to eight): their ids and styles, and up to nine vibration devices each with their
// information (8 bytes each).
struct UnkPadSet {
    u8 pad_0x0[0x44];
    u32 count;
    nn::hid::NpadIdType ids[9];                     // 0x48
    nn::hid::VibrationDeviceHandle handles[9][9];  // 0x6c
    u8 infos[9][9][8];                              // 0x1b0
    u8 pad_0x438[0x24];
    u32 styles[8];  // 0x45c
};
static_assert(__builtin_offsetof(UnkPadSet, handles) == 0x6c, "UnkPadSet placeholder layout");
static_assert(__builtin_offsetof(UnkPadSet, infos) == 0x1b0, "UnkPadSet placeholder layout");
static_assert(__builtin_offsetof(UnkPadSet, styles) == 0x45c, "UnkPadSet placeholder layout");

// Gets each pad's vibration devices and initialises them, keeping their information.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_710006ddc8(UnkPadSet* self) {
    for (u64 i = 0; i < self->count; ++i) {
        nn::hid::NpadStyleSet styles;
        __builtin_memcpy(&styles, &self->styles[i], sizeof(u32));
        s32 n = nn::hid::GetVibrationDeviceHandles(self->handles[i], 9, self->ids[i], styles);
        for (s32 j = 0; j < n; ++j) {
            nn::hid::VibrationDeviceHandle handle = self->handles[i][j];
            nn::hid::InitializeVibrationDevice(handle);
            nn::hid::GetVibrationDeviceInfo(reinterpret_cast<nn::hid::VibrationDeviceInfo*>(self->infos[i][j]), handle);
        }
    }
}

// ---- fn_710006deac: the pads' per-frame update ----

#include <math.h>

#include "base/UnkVector.h"
#include "base/input/UnkRumbleManager.h"

using base::input::UnkRumbleManager;

// The rumble manager (created on first use) and its target pad's two vibration devices.
extern UnkRumbleManager* g_unk_7101d47348;
void fn_710006fb30(UnkRumbleManager* self, nn::hid::VibrationDeviceHandle left,
                   nn::hid::VibrationDeviceHandle right);

// What the controller applet pauses: the game (fn_7100132a70), the sound manager (fn_7100108b48) and
// the scene state (fn_7100f022c4).
void fn_7100132a70(bool paused);
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_7100108b48(UnkSoundMgr* self, bool paused);
struct UnkStruct_0x128;  // the scene state (src/game/UnkLoadingScreen.cpp)
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100f022c4(UnkStruct_0x128* self, bool paused);
void fn_710006d55c(UnkPadSet* self);  // the frame's remaining input work

static inline UnkStruct_0x128* PadSceneState() {
    if (!g_unk_7101d47320) {
        auto* s = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(s);
        g_unk_7101d47320 = s;
    }
    return g_unk_7101d47320;
}

template <class T>
static inline T& PadField(UnkPadSet* self, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<u8*>(self) + offset);
}

static inline u32 StyleBits(const nn::hid::NpadStyleSet& s) {
    u32 v;
    __builtin_memcpy(&v, &s, sizeof(v));
    return v;
}

template <class R>
static inline bool IsSuccess(const R& r) {
    u32 v;
    __builtin_memcpy(&v, &r, sizeof(v));
    return v == 0;
}

// Makes `pad` the active pad and points the rumble at its devices (pads 0 and 1 only).
static inline void SetActivePad(UnkPadSet* self, u32 pad) {
    s32& active = PadField<s32>(self, 0x40);
    if ((u32)active == pad)
        return;
    active = pad;
    if (pad >= 2)
        return;
    if (!g_unk_7101d47348) {
        auto* m = static_cast<UnkRumbleManager*>(fn_71000839a0(0x2210, 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    fn_710006fb30(g_unk_7101d47348, self->handles[pad][0], self->handles[pad][1]);
}

// A stick: its raw axes (and the previous ones, which a change of sign resets), the deadzoned
// value (radius 0.3 to 0.97 mapped to 0 to 1) and whether each axis is moved.
static inline void ReadStick(const s32* raw, s32* prev_x, s32* prev_y, f32* out, bool* moved_x, bool* moved_y) {
    s32 x = raw[0];
    s32 y = raw[1];
    s32 px = x * *prev_x;
    s32 py = y * *prev_y;
    *prev_x = px < 0 ? 0 : x;
    *prev_y = py < 0 ? 0 : y;
    out[0] = 0.0f;
    out[1] = 0.0f;
    f32 fx = (f32)x / 32767.0f;
    f32 fy = (f32)y / 32767.0f;
    f32 m2 = fx * fx + fy * fy;
    f32 vy = 0.0f;
    if (m2 >= 0.09f) {
        f32 m = sqrtf(m2);
        f32 r = m > 0.97f ? 0.97f : m;
        if (m != 0.0f) {
            fx = fx * (1.0f / m);
            fy = fy * (1.0f / m);
        }
        f32 s = (r - 0.3f) * 1.4925373f;
        if (px >= 0)
            out[0] = s * fx;
        if (py < 0) {
            vy = out[1];
        } else {
            vy = s * fy;
            out[1] = vy;
        }
    }
    *moved_x = out[0] != 0.0f;
    *moved_y = vy != 0.0f;
}

// Each frame: picks the active pad (the only connected one, or the one input comes from), reads the
// pads' states and sticks, and when no pad is active shows the controller applet (with the game,
// its sound and the scene paused) and takes the pad it chooses.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
// The original reads the buttons of a pad with neither style through a null pointer; that read is
// skipped here.
void fn_710006deac(UnkPadSet* self) {
    s32& active = PadField<s32>(self, 0x40);
    bool loop = false;
    if (active < 0) {
        if (self->count != 0) {
            s32 first = -1;
            s32 connected = 0;
            for (u32 i = 0; i < self->count; ++i) {
                u32 s = StyleBits(nn::hid::GetNpadStyleSet(self->ids[i]));
                self->styles[i] = s;
                if (s) {
                    if (first < 0)
                        first = i;
                    ++connected;
                }
            }
            if (connected == 1)
                SetActivePad(self, first);
        }
    }
    if (active >= 0) {
        u32 s = StyleBits(nn::hid::GetNpadStyleSet(self->ids[active]));
        self->styles[active] = s;
        if (s)
            loop = true;
        else
            active = -1;
    }
    if (loop) {
        if (self->count == 0) {
            fn_710006d55c(self);
            return;
        }
        for (u32 i = 0; i < self->count; ++i) {
            u8* stick = reinterpret_cast<u8*>(self) + 0x854 + i * 0x14;  // 4 moved flags, then 4 axes
            __builtin_memcpy(reinterpret_cast<u8*>(self) + 0x7a0 + i * 0x14, stick, 0x14);
            PadField<u32>(self, 0x438 + i * 4) = self->styles[i];
            u32 s = StyleBits(nn::hid::GetNpadStyleSet(self->ids[i]));
            self->styles[i] = s;
            if (!s)
                continue;
            u8* full = reinterpret_cast<u8*>(self) + 0x638 + i * 0x28;
            u8* handheld = reinterpret_cast<u8*>(self) + 0x4a8;
            if (s & 1) {
                __builtin_memcpy(reinterpret_cast<u8*>(self) + 0x4d0 + i * 0x28, full, 0x28);
                nn::hid::GetNpadState(reinterpret_cast<nn::hid::NpadFullKeyState*>(full), self->ids[i]);
                s = self->styles[i];
            }
            if (s & 2) {
                __builtin_memcpy(reinterpret_cast<u8*>(self) + 0x480, handheld, 0x28);
                nn::hid::GetNpadState(reinterpret_cast<nn::hid::NpadHandheldState*>(handheld), self->ids[i]);
                s = self->styles[i];
            }
            bool* moved = reinterpret_cast<bool*>(stick);
            f32* axes = reinterpret_cast<f32*>(stick + 4);
            const u8* state = (s & 1) ? full : (s & 2) ? handheld : nullptr;
            if (state) {
                ReadStick(reinterpret_cast<const s32*>(state + 0x10), &PadField<s32>(self, 0x908 + i * 4),
                          &PadField<s32>(self, 0x92c + i * 4), axes, &moved[0], &moved[1]);
                s = self->styles[i];
                state = (s & 1) ? full : (s & 2) ? handheld : nullptr;
            }
            if (state) {
                ReadStick(reinterpret_cast<const s32*>(state + 0x18), &PadField<s32>(self, 0x950 + i * 4),
                          &PadField<s32>(self, 0x974 + i * 4), axes + 2, &moved[2], &moved[3]);
                s = self->styles[i];
                state = (s & 1) ? full : (s & 2) ? handheld : nullptr;
            }
            const u64* buttons = state ? reinterpret_cast<const u64*>(state + 8) : nullptr;
            bool input = moved[0] || moved[1] || moved[2] || moved[3] || (buttons && *buttons != 0);
            if (input && i != (u32)active && active != -1)
                active = -1;
        }
        if (active >= 0) {
            fn_710006d55c(self);
            return;
        }
    }
    nn::hid::SetNpadJoyHoldType(static_cast<nn::hid::NpadJoyHoldType>(1));
    alignas(8) u8 arg[0x4a0];
    auto& support = *reinterpret_cast<nn::hid::ControllerSupportArg*>(arg);
    UnkControllerSupportArgSetDefault(&support);
    arg[0] = 1;  // the minimum and maximum player counts
    arg[1] = 1;
    arg[3] = 0;  // no left justification
    arg[5] = 1;  // single mode
    fn_7100132a70(true);
    fn_7100108b48(g_unk_7101d4d820, true);
    fn_7100f022c4(PadSceneState(), true);
    alignas(8) u8 result[0x10];
    auto r = nn::hid::ShowControllerSupport(reinterpret_cast<nn::hid::ControllerSupportResultInfo*>(result), support);
    fn_7100132a70(false);
    fn_7100f022c4(PadSceneState(), false);
    fn_7100108b48(g_unk_7101d4d820, false);
    if (IsSuccess(r)) {
        u32 chosen = *reinterpret_cast<const u32*>(result + 4);
        for (u32 i = 0; i < self->count; ++i) {
            if (self->ids[i] == chosen) {
                SetActivePad(self, i);
                break;
            }
        }
    }
    fn_710006d55c(self);
}
