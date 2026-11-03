// Unit 47 of tools/tu_map.py (no class hint): points the rumble manager's two vibration targets at
// the target pad's two vibration devices (left, then right). Names are placeholders.
#include <nn/hid.h>

#include "base/input/UnkRumbleManager.h"
#include "types.h"

using base::input::UnkRumbleManager;

void fn_710006fb30(UnkRumbleManager* self, nn::hid::VibrationDeviceHandle left,
                   nn::hid::VibrationDeviceHandle right) {
    self->targets[0].SetVibrationDeviceHandle(left);
    self->targets[1].SetVibrationDeviceHandle(right);
}
