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
// A controller sample as both SDKs lay it out (nn::hid's NpadFullKeyState and NpadHandheldState).
struct UnkPadState {
    s64 sampling_number;
    u64 buttons;
    s32 stick_l[2];
    s32 stick_r[2];
    u32 attributes;
};
static_assert(sizeof(UnkPadState) == 0x28, "UnkPadState layout");

// A pad's sticks as the game reads them: whether each axis moved, then the deadzoned axes.
struct UnkPadSticks {
    bool moved[4];
    f32 axes[4];
};
static_assert(sizeof(UnkPadSticks) == 0x14, "UnkPadSticks layout");

struct UnkPadSet {
    u8 pad_0x0[0x40];
    s32 active;  // 0x40, the pad input comes from (-1: none)
    u32 count;
    nn::hid::NpadIdType ids[9];                     // 0x48
    nn::hid::VibrationDeviceHandle handles[9][9];  // 0x6c
    u8 infos[9][9][8];                              // 0x1b0
    u32 prev_styles[9];                             // 0x438
    u32 styles[9];                                  // 0x45c
    UnkPadState prev_handheld;                      // 0x480
    UnkPadState handheld;                           // 0x4a8
    UnkPadState prev_full[9];                       // 0x4d0
    UnkPadState full[9];                            // 0x638
    UnkPadSticks prev_sticks[9];                    // 0x7a0
    UnkPadSticks sticks[9];                         // 0x854
    s32 prev_l[2][9];                               // 0x908: each axis's last raw value
    s32 prev_r[2][9];                               // 0x950
};
static_assert(__builtin_offsetof(UnkPadSet, full) == 0x638, "UnkPadSet placeholder layout");
static_assert(__builtin_offsetof(UnkPadSet, prev_l) == 0x908, "UnkPadSet placeholder layout");
static_assert(__builtin_offsetof(UnkPadSet, handles) == 0x6c, "UnkPadSet placeholder layout");
static_assert(__builtin_offsetof(UnkPadSet, infos) == 0x1b0, "UnkPadSet placeholder layout");
static_assert(__builtin_offsetof(UnkPadSet, styles) == 0x45c, "UnkPadSet placeholder layout");

static inline nn::hid::NpadStyleSet StyleSet(const u32& bits) {
    nn::hid::NpadStyleSet styles;
    __builtin_memcpy(&styles, &bits, sizeof(u32));
    return styles;
}

// Gets each pad's vibration devices and initialises them, keeping their information.
void fn_710006ddc8(UnkPadSet* self) {
    for (u64 i = 0; i < self->count; ++i) {
        u32 n = nn::hid::GetVibrationDeviceHandles(self->handles[i], 9, self->ids[i], StyleSet(self->styles[i]));
        for (u32 j = 0; j < n; ++j) {
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
static inline void SetActivePad(UnkPadSet* self, s32 pad) {
    if (self->active == pad)
        return;
    self->active = pad;
    if ((u32)pad > 1)
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
static inline void ReadStick(const s32* raw, s32* prev_x, s32* prev_y, f32* out, bool* moved) {
    s32 px = raw[0] * *prev_x;
    s32 py = raw[1] * *prev_y;
    *prev_x = px >= 0 ? raw[0] : 0;
    *prev_y = py < 0 ? 0 : raw[1];
    out[0] = 0.0f;
    out[1] = 0.0f;
    f32 fx = (f32)raw[0] / 32767.0f;
    f32 fy = (f32)raw[1] / 32767.0f;
    f32 m2 = fx * fx + fy * fy;
    if (m2 >= 0.09f) {
        f32 root = sqrtf(m2);
        f32 r = 0.97f < root ? 0.97f : root;
        f32 m = sqrtf(m2);
        if (m != 0.0f) {
            f32 inv = 1.0f / m;
            fx = fx * inv;
            fy = fy * inv;
        }
        f32 s = (r - 0.3f) * 1.4925373f;
        if (px >= 0)
            out[0] = s * fx;
        if (py >= 0)
            out[1] = s * fy;
    }
    moved[0] = out[0] != 0.0f;
    moved[1] = out[1] != 0.0f;
}

// Each frame: picks the active pad (the only connected one, or the one input comes from), reads the
// pads' states and sticks, and when no pad is active shows the controller applet (with the game,
// its sound and the scene paused) and takes the pad it chooses. As in the original, a pad with
// neither style bit has its buttons read through a null pointer (the port's one pad is handheld).
// NON_MATCHING(minor): the same loads, stores, calls and tests, but the pad loop's induction
// variables are assigned differently (the original addresses the previous styles from the index
// and everything else from one running offset), which shifts registers, spills a stick product
// around the sqrtf calls and adds a few moves; the frame is 0x10 bytes larger.
static inline void ShowApplet(UnkPadSet* self);

void fn_710006deac(UnkPadSet* self) {
    if (self->active < 0 && self->count != 0) {
        s32 connected = 0;
        s32 first = -1;
        for (u64 i = 0; i < self->count; ++i) {
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
    if (self->active >= 0) {
        s32 a = self->active;
        u32 s = StyleBits(nn::hid::GetNpadStyleSet(self->ids[a]));
        self->styles[a] = s;
        if (!s) {
            if (self->active != -1)
                self->active = -1;
        } else if (self->active >= 0) {
            for (u64 i = 0; i < self->count; ++i) {
                self->prev_sticks[i] = self->sticks[i];
                self->prev_styles[i] = self->styles[i];
                u32 st = StyleBits(nn::hid::GetNpadStyleSet(self->ids[i]));
                self->styles[i] = st;
                if (!st)
                    continue;
                if (st & 1) {
                    self->prev_full[i] = self->full[i];
                    nn::hid::GetNpadState(reinterpret_cast<nn::hid::NpadFullKeyState*>(&self->full[i]), self->ids[i]);
                    st = self->styles[i];
                }
                if (st & 2) {
                    self->prev_handheld = self->handheld;
                    nn::hid::GetNpadState(reinterpret_cast<nn::hid::NpadHandheldState*>(&self->handheld),
                                          self->ids[i]);
                    st = self->styles[i];
                }
                UnkPadSticks& sticks = self->sticks[i];
                const s32* stick_l = (st & 1) ? self->full[i].stick_l : (st & 2) ? self->handheld.stick_l : nullptr;
                if (stick_l) {
                    ReadStick(stick_l, &self->prev_l[0][i], &self->prev_l[1][i], &sticks.axes[0], &sticks.moved[0]);
                    st = self->styles[i];
                }
                const s32* stick_r = (st & 1) ? self->full[i].stick_r : (st & 2) ? self->handheld.stick_r : nullptr;
                if (stick_r) {
                    ReadStick(stick_r, &self->prev_r[0][i], &self->prev_r[1][i], &sticks.axes[2], &sticks.moved[2]);
                    st = self->styles[i];
                }
                const u64* buttons = (st & 1) ? &self->full[i].buttons : (st & 2) ? &self->handheld.buttons : nullptr;
                if (sticks.moved[0] || sticks.moved[1] || sticks.moved[2] || sticks.moved[3] || *buttons != 0) {
                    if (i != (u32)self->active && self->active != -1)
                        self->active = -1;
                }
            }
        }
    }
    if (self->active < 0)
        ShowApplet(self);
    fn_710006d55c(self);
}

// Shows the controller applet (with the game, its sound and the scene paused) and takes the pad
// it chooses.
static inline void ShowApplet(UnkPadSet* self) {
    nn::hid::SetNpadJoyHoldType(static_cast<nn::hid::NpadJoyHoldType>(1));
    alignas(8) u8 arg[0x438];
    auto& support = *reinterpret_cast<nn::hid::ControllerSupportArg*>(arg);
    UnkControllerSupportArgSetDefault(&support);
    arg[0] = 1;  // the minimum and maximum player counts
    arg[1] = 1;
    arg[3] = 0;  // no left justification
    arg[5] = 1;  // single mode
    fn_7100132a70(true);
    fn_7100108b48(g_unk_7101d4d820, true);
    fn_7100f022c4(PadSceneState(), true);
    alignas(8) u8 result[0x8];
    auto r = nn::hid::ShowControllerSupport(reinterpret_cast<nn::hid::ControllerSupportResultInfo*>(result), support);
    fn_7100132a70(false);
    fn_7100f022c4(PadSceneState(), false);
    fn_7100108b48(g_unk_7101d4d820, false);
    if (IsSuccess(r)) {
        u32 chosen = *reinterpret_cast<const u32*>(result + 4);
        u64 k = 0;
        for (u32 i = 0; i < self->count; ++i, ++k) {
            if (self->ids[k] == chosen) {
                SetActivePad(self, k);
                break;
            }
        }
    }
}
