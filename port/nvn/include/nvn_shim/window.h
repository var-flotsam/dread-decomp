#pragma once

// The NVN window (src/window.cpp has what each function does so far).

#include <cstdint>

namespace dread::nvn {

// What the shim has seen a window do (for checks).
struct WindowStats {
    bool exists = false;
    std::int32_t textures = 0, present_interval = 0;
    std::uint64_t acquires = 0;
    void* native_window = nullptr;
};
WindowStats GetWindowStats(const void* window);

}  // namespace dread::nvn
