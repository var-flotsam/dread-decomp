#pragma once

// The shim's state for the game's NVN objects. NVN objects live in storage the game owns (in its
// own structs, on its stack for builders); the shim never writes into that storage, because the
// game keeps its own fields right next to it (the engine reads its pool's CPU base at the NVN memory
// pool + 0x100, fn_7100160f60). Instead each object's state lives in a side table keyed by the
// storage address and the kind: SetDefaults (builders) and Initialize (objects) create or reset the
// entry, Finalize removes it. A builder on the stack that is never finalised keeps its entry until
// the address is reused, which resets it.

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

namespace dread::nvn::detail {

// One lock for all of the shim's object state (procs.cpp). Recursive: recording a draw holds it while
// the binding layer's resolver, which takes it too, looks up buffers and textures.
std::recursive_mutex& StateLock();

// Each part registers its functions with RegisterProc; the entry layer calls these once, before the
// first lookup (a static library would drop a part whose registrations ran from static initialisers).
void RegisterDeviceProcs();    // device.cpp
void RegisterMemoryProcs();    // memory.cpp: memory pools, buffers
void RegisterQueueProcs();     // queue.cpp: queues, syncs
void RegisterCommandProcs();   // command.cpp: command buffers
void RegisterStateProcs();     // state.cpp: the fixed-function state objects
void RegisterWindowProcs();    // window.cpp
void RegisterTextureProcs();   // texture.cpp: textures, texture and sampler pools, samplers
void RegisterProgramProcs();   // program.cpp: programs

// Releases the Vulkan objects of every memory pool (memory.cpp); nvnDeviceFinalize calls it before
// it destroys the backend. The pools stay; their Vulkan buffers are made again on next use.
void ReleaseMemoryVulkanObjects();
// The same for textures, pool samplers and the resolver's placeholders (texture.cpp).
void ReleaseTextureVulkanObjects();
// The draw path's objects: command buffers, descriptor pools, pipelines (command.cpp), and the
// programs' modules and layouts (program.cpp). nvnDeviceFinalize calls it first, after the GPU is idle.
void ReleaseDrawVulkanObjects();

// nvnQueueSubmitCommands: submits the recordings behind the game's command handles, in order, on the
// queue's timeline (command.cpp); returns the timeline value it signals, 0 if nothing was submitted.
std::uint64_t SubmitRecordings(std::int32_t count, const std::uint64_t* handles);
// Runs `destroy` (it frees Vulkan objects the game finalised) once no recording that may name them
// is open or unsubmitted and the GPU has passed them (command.cpp); at once when nothing is pending.
void DeferDestroy(std::function<void()> destroy);
// nvnQueueFenceSync and nvnWindowAcquireTexture: the sync becomes signalled when the GPU reaches what
// is submitted now (queue.cpp).
void FenceSyncAtLastSubmit(void* sync);
// nvnQueuePresentTexture: the window's texture `index` was presented (window.cpp).
void WindowPresented(const void* window, std::int32_t index);

template <class T>
class Registry {
public:
    // The state at `storage`, created (default-constructed) when missing.
    T& Get(const void* storage) {
        std::unique_ptr<T>& slot = map_[storage];
        if (!slot)
            slot = std::make_unique<T>();
        return *slot;
    }
    // The state at `storage`, null when the game never created one there.
    T* Find(const void* storage) {
        auto it = map_.find(storage);
        return it == map_.end() ? nullptr : it->second.get();
    }
    // A fresh state at `storage` (SetDefaults, Initialize).
    T& Reset(const void* storage) {
        std::unique_ptr<T>& slot = map_[storage];
        slot = std::make_unique<T>();
        return *slot;
    }
    // A builder's SetDefaults: fresh state, but the device it was given stays. The game sets the
    // device first and the defaults after on the queue, buffer and texture builders
    // (fn_71001587f0), and the buffer builder's defaults again per buffer (fn_710015c9b0).
    T& ResetKeepingDevice(const void* storage) {
        const void* device = nullptr;
        if (T* old = Find(storage))
            device = old->device;
        T& fresh = Reset(storage);
        fresh.device = device;
        return fresh;
    }
    void Erase(const void* storage) { map_.erase(storage); }
    template <class F>
    void ForEach(F&& f) {
        for (auto& kv : map_)
            f(kv.first, *kv.second);
    }
    std::size_t size() const { return map_.size(); }

private:
    std::map<const void*, std::unique_ptr<T>> map_;
};

}  // namespace dread::nvn::detail
