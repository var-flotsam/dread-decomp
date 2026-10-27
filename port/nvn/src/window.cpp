// The NVN window. Signatures are those of fn_71001587f0 (builder, Initialize, Finalize,
// SetPresentInterval, AcquireTexture) and the present fn_7100159bf0 (AcquireTexture after each
// nvnQueuePresentTexture). The game gives the window two render targets it created itself
// (WindowBuilderSetTextures(builder, 2, textures)) and the native window nn::vi gave it.
//
// The window has no Vulkan surface yet (the textures it is given are not Vulkan images until the
// texture part exists, and the port's window is nn::vi's): AcquireTexture hands out the textures in
// turn and leaves the sync it is given alone (every sync wait returns at once, queue.cpp), and a
// present only counts.

#include <cstdio>
#include <vector>

#include "nvn_shim/procs.h"
#include "nvn_shim/window.h"
#include "objects.h"

namespace dread::nvn {

namespace {

struct WindowBuilder {
    const void* device = nullptr;
    void* native_window = nullptr;
    std::vector<const void*> textures;
    std::int32_t present_interval = 1;
};

struct Window {
    void* native_window = nullptr;
    std::vector<const void*> textures;
    std::int32_t present_interval = 1;
    std::int32_t next = 0;
    std::uint64_t acquires = 0;
};

detail::Registry<WindowBuilder> g_builders;
detail::Registry<Window> g_windows;

void WindowBuilderSetDefaults(void* builder) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.ResetKeepingDevice(builder);
}
const void* WindowBuilderGetDevice(const void* builder) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    WindowBuilder* b = g_builders.Find(builder);
    return b ? b->device : nullptr;
}
void WindowBuilderSetDevice(void* builder, const void* device) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Get(builder).device = device;
}
void WindowBuilderSetNativeWindow(void* builder, void* native_window) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Get(builder).native_window = native_window;
}
void WindowBuilderSetTextures(void* builder, std::int32_t count, const void* const* textures) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    WindowBuilder& b = g_builders.Get(builder);
    b.textures.assign(textures, textures + (count > 0 && textures ? count : 0));
}
void WindowBuilderSetPresentInterval(void* builder, std::int32_t interval) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_builders.Get(builder).present_interval = interval;
}

std::int32_t WindowInitialize(void* window, const void* builder) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    WindowBuilder* b = g_builders.Find(builder);
    if (!b || b->textures.empty()) {
        std::fprintf(stderr, "nvn shim: nvnWindowInitialize without textures\n");
        return 0;
    }
    Window& w = g_windows.Reset(window);
    w.native_window = b->native_window;
    w.textures = b->textures;
    w.present_interval = b->present_interval;
    return 1;
}

void WindowFinalize(void* window) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_windows.Erase(window);
}

void WindowSetPresentInterval(void* window, std::int32_t interval) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    g_windows.Get(window).present_interval = interval;
}

std::int32_t WindowGetPresentInterval(const void* window) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    Window* w = g_windows.Find(window);
    return w ? w->present_interval : 0;
}

// Returns 0 (the shim's success; the game does not look at it). The sync is the one the game waits
// on before rendering into the texture; nothing is in flight, so it needs no signal yet.
std::int32_t WindowAcquireTexture(void* window, void*, std::int32_t* index) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    Window* w = g_windows.Find(window);
    if (!w || w->textures.empty()) {
        if (index)
            *index = 0;
        return 1;
    }
    if (index)
        *index = w->next;
    w->next = (w->next + 1) % static_cast<std::int32_t>(w->textures.size());
    w->acquires++;
    return 0;
}

std::int32_t WindowGetNumTextures(const void* window) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    Window* w = g_windows.Find(window);
    return w ? static_cast<std::int32_t>(w->textures.size()) : 0;
}

void WindowSetDebugLabel(void*, const char*) {}

}  // namespace

WindowStats GetWindowStats(const void* window) {
    std::lock_guard<std::mutex> lock(detail::StateLock());
    WindowStats out;
    if (Window* w = g_windows.Find(window)) {
        out.exists = true;
        out.textures = static_cast<std::int32_t>(w->textures.size());
        out.present_interval = w->present_interval;
        out.acquires = w->acquires;
        out.native_window = w->native_window;
    }
    return out;
}

void detail::RegisterWindowProcs() {
    Register("nvnWindowBuilderSetDefaults", &WindowBuilderSetDefaults);
    Register("nvnWindowBuilderGetDevice", &WindowBuilderGetDevice);
    Register("nvnWindowBuilderSetDevice", &WindowBuilderSetDevice);
    Register("nvnWindowBuilderSetNativeWindow", &WindowBuilderSetNativeWindow);
    Register("nvnWindowBuilderSetTextures", &WindowBuilderSetTextures);
    Register("nvnWindowBuilderSetPresentInterval", &WindowBuilderSetPresentInterval);
    Register("nvnWindowInitialize", &WindowInitialize);
    Register("nvnWindowFinalize", &WindowFinalize);
    Register("nvnWindowSetPresentInterval", &WindowSetPresentInterval);
    Register("nvnWindowGetPresentInterval", &WindowGetPresentInterval);
    Register("nvnWindowAcquireTexture", &WindowAcquireTexture);
    Register("nvnWindowGetNumTextures", &WindowGetNumTextures);
    Register("nvnWindowSetDebugLabel", &WindowSetDebugLabel);
}

}  // namespace dread::nvn
