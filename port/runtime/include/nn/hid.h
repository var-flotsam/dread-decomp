#pragma once

// nn::hid: controllers and vibration. The input object's constructor fn_710006daf0 initialises
// the Npad layer (InitializeNpad, SetSupportedNpadStyleSet(3): full key and handheld,
// SetSupportedNpadIdType with nine ids, 0x20 for handheld first, then 0 to 7); its poll
// fn_710006deac reads each id's style set every frame and the full-key or handheld state of the
// ones connected, and opens the controller applet (ShowControllerSupport) when it needs one;
// fn_710006ddc8 sets up the vibration devices. Vibration (include/base/input/UnkHidVibration.h:
// the mixers, targets and node connections of the rumble manager, fn_71000733e4, and the players,
// fn_7100070740) is gameplay. On PC: one handheld controller, always connected, whose buttons and
// sticks the port's window sets (dread_port_set_npad_state); no vibration devices, so the
// vibration graph runs empty.
//
// `main` imports (data/functions/; the parameter lists from the imports' mangled names):
// InitializeNpad, SetSupportedNpadStyleSet, SetSupportedNpadIdType, SetNpadJoyHoldType,
// GetNpadStyleSet, GetNpadState (full key, handheld), GetVibrationDeviceHandles,
// GetVibrationDeviceInfo, InitializeVibrationDevice, ShowControllerSupport,
// ControllerSupportArg::SetDefault, and the vibration classes' members (VibrationMixer,
// VibrationTarget, VibrationNodeConnection, VibrationPlayer, VibrationNode::Update). All declared
// here or in base/input/UnkHidVibration.h; the data import VibrationNode::DefaultVibrationSampleInterval
// is not (nothing the port compiles reads it).

#include "base/input/UnkHidVibration.h"
#include "nn/nn_common.h"

namespace nn::util {

template <int N>
struct BitFlagStorage;
template <>
struct BitFlagStorage<32> {
    using Type = u32;
};
template <>
struct BitFlagStorage<64> {
    using Type = u64;
};

// A fixed-size set of flags: the style set travels as one word (the mangled name gives the
// template's parameters: an int and a tag type).
template <int N, typename Tag>
struct BitFlagSet {
    typename BitFlagStorage<N>::Type bits;
};

}  // namespace nn::util

namespace nn::hid {

// An Npad id: 0 to 7 for the controllers, 0x20 for the console's own (handheld).
using NpadIdType = u32;
constexpr NpadIdType NpadId_Handheld = 0x20;

struct NpadStyleTag;
using NpadStyleSet = nn::util::BitFlagSet<32, NpadStyleTag>;
// fn_710006deac reads the full-key state on bit 0 and the handheld state on bit 1.
constexpr u32 NpadStyle_FullKey = 1u << 0;
constexpr u32 NpadStyle_Handheld = 1u << 1;

enum NpadJoyHoldType : s64 {};

struct AnalogStickState {
    s32 x;
    s32 y;
};

// One controller sample: fn_710006deac keeps the previous one by copying five words. The field
// order inside them is the SDK's convention (a sampling number, the buttons, two sticks, the
// attributes), not something the game shows yet; the port fills them the same way for both styles.
struct NpadState {
    s64 sampling_number;
    u64 buttons;
    AnalogStickState analog_stick_l;
    AnalogStickState analog_stick_r;
    u32 attributes;
};
static_assert(sizeof(NpadState) == 0x28, "fn_710006deac copies five words");
constexpr u32 NpadAttribute_IsConnected = 1u << 0;
struct NpadFullKeyState : NpadState {};
struct NpadHandheldState : NpadState {};

void InitializeNpad();
void SetSupportedNpadStyleSet(NpadStyleSet styles);
void SetSupportedNpadIdType(const NpadIdType* ids, u64 count);
void SetNpadJoyHoldType(NpadJoyHoldType type);
NpadStyleSet GetNpadStyleSet(const NpadIdType& id);
void GetNpadState(NpadFullKeyState* out, const NpadIdType& id);
void GetNpadState(NpadHandheldState* out, const NpadIdType& id);

// The controller applet's argument, in fn_710006deac's frame; the port never reads it, so its
// size does not matter here.
struct ControllerSupportArg {
    void SetDefault();
};
// What the applet returns: fn_710006deac reads the word after the count as the chosen id.
struct ControllerSupportResultInfo {
    s32 player_count;
    NpadIdType selected_id;
};
Result ShowControllerSupport(ControllerSupportResultInfo* out, const ControllerSupportArg& arg);

// A vibration device: one word (fn_710006ddc8 copies it to its frame).
struct VibrationDeviceHandle {
    u32 value;
};
struct VibrationDeviceInfo;
s32 GetVibrationDeviceHandles(VibrationDeviceHandle* out, s32 max, const NpadIdType& id, NpadStyleSet styles);
void InitializeVibrationDevice(const VibrationDeviceHandle& handle);
void GetVibrationDeviceInfo(VibrationDeviceInfo* out, const VibrationDeviceHandle& handle);

// The vibration file's header, as VibrationPlayer::GetFileInfo returns it (more than 16 bytes:
// through x8). The game reads the sample length at +0xc; the rest is the format's.
struct VibrationFileInfo {
    u32 meta_data_size;
    u16 format_id;
    u16 sampling_rate;
    u32 data_size;
    s32 sample_length;
    s32 loop_start_position;
    s32 loop_end_position;
    s32 loop_interval;
};
static_assert(__builtin_offsetof(VibrationFileInfo, sample_length) == 0xc, "fn_7100070a28 reads +0xc");

}  // namespace nn::hid
