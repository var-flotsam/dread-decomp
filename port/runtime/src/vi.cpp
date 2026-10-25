// nn::vi for the port: one display, one layer, and no native window until the port has a window
// system (M2).

#include "nn/vi.h"

namespace nn::vi {

struct Display {
    s32 index;
};

struct Layer {
    Display* display;
    void* native_window;
};

namespace {
Display s_default_display{0};
Layer s_layer{&s_default_display, nullptr};
}  // namespace

void Initialize() {
    // TODO(M2): fn_71001587f0 (graphics start-up). Bring up the window system here (SDL or the
    // platform API).
}

Result OpenDefaultDisplay(Display** out_display) {
    // TODO(M2): fn_71001587f0. The primary monitor.
    *out_display = &s_default_display;
    return ResultSuccess();
}

Result CreateLayer(Layer** out_layer, Display* display) {
    // TODO(M2): fn_71001587f0. Open the port's window on the display.
    s_layer.display = display;
    *out_layer = &s_layer;
    return ResultSuccess();
}

Result GetNativeWindow(void** out_window, Layer* layer) {
    // TODO(M2): fn_71001587f0 hands this to NVN's window builder, so the NVN shim decides what
    // it is (the window handle or a Vulkan surface). There is none yet, so this fails.
    *out_window = layer ? layer->native_window : nullptr;
    return *out_window ? ResultSuccess() : ResultNotPorted();
}

}  // namespace nn::vi
