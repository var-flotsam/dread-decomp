#pragma once

// The shim's state for the game's NVN objects. NVN objects live in storage the game owns (in its
// own structs, on its stack for builders); the shim never writes into that storage, because the
// game keeps its own fields right next to it (the engine reads its pool's CPU base at the NVN memory
// pool + 0x100, fn_7100160f60). Instead each object's state lives in a side table keyed by the
// storage address and the kind: SetDefaults (builders) and Initialize (objects) create or reset the
// entry, Finalize removes it. A builder on the stack that is never finalised keeps its entry until
// the address is reused, which resets it.

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

namespace dread::nvn::detail {

std::mutex& StateLock();  // one lock for all of the shim's object state (procs.cpp)

// Each part registers its functions with RegisterProc; the entry layer calls these once, before the
// first lookup (a static library would drop a part whose registrations ran from static initialisers).
void RegisterDeviceProcs();    // device.cpp
void RegisterMemoryProcs();    // memory.cpp: memory pools, buffers
void RegisterQueueProcs();     // queue.cpp: queues, syncs
void RegisterCommandProcs();   // command.cpp: command buffers
void RegisterStateProcs();     // state.cpp: the fixed-function state objects
void RegisterWindowProcs();    // window.cpp

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
