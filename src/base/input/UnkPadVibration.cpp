// From unit 46 of tools/tu_map.py (0x710006d2a0-0x710006f390, no class hint): the controllers'
// part of the input system (each pad's Npad id, style and vibration devices). Names and layouts are
// placeholders.
// Holds fn_710006ddc8 (sets up the vibration devices).
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
}  // namespace nn::hid
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
