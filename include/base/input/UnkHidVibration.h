#pragma once

// The nn::hid vibration classes the game embeds by value. lib/nnsdk has no declarations for them;
// these are opaque stand-ins with the library's constructors and destructors (symbols
// nn::hid::<T>::<T>() and ~<T>()), sized from the offsets of the members that follow them
// (src/base/input/CRumbleManager.cpp, src/game_s1/fn_71000707dc.cpp). The port's runtime lists the
// same classes in port/runtime/include/nn/hid.h as still to be declared: move these there when the
// port needs their bodies.

#include "types.h"

namespace nn::hid {

class VibrationMixer {
public:
    VibrationMixer();
    u8 storage[0x48];
};

class VibrationTarget {
public:
    VibrationTarget();
    u8 storage[0x50];
};

class VibrationNodeConnection {
public:
    VibrationNodeConnection();
    ~VibrationNodeConnection();
    u8 storage[0x38];
};

}  // namespace nn::hid
