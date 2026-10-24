#pragma once

// nn::vi: the display and the layer the game draws into. The graphics start-up
// (src/game_s7/fn_71001587f0.cpp) initialises the library, opens the default display, creates
// one layer on it and asks for the layer's native window, which it hands to NVN's window
// builder. On PC the native window is the port's own window and the NVN shim presents into it
// (M2). `main` imports (data/functions/): Initialize, OpenDefaultDisplay, CreateLayer,
// GetNativeWindow.

#include "nn/nn_common.h"

namespace nn::vi {

struct Display;  // opaque: the port's monitor record
struct Layer;    // opaque: the port's window

void Initialize();
Result OpenDefaultDisplay(Display** out_display);
Result CreateLayer(Layer** out_layer, Display* display);
Result GetNativeWindow(void** out_window, Layer* layer);

}  // namespace nn::vi
