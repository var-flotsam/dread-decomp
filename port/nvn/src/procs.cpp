// The entry layer (include/nvn_shim/procs.h).

#include "nvn_shim/procs.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <map>
#include <mutex>
#include <utility>

#include "objects.h"

namespace dread::nvn {

std::mutex& detail::StateLock() {
    static std::mutex m;
    return m;
}

namespace {

// Function-local statics: the table is used from the registration functions and from lookups.
std::recursive_mutex& Lock() {
    static std::recursive_mutex m;
    return m;
}
std::map<std::string, Proc>& Table() {
    static std::map<std::string, Proc> t;
    return t;
}
std::vector<std::string>& Duplicates() {
    static std::vector<std::string> d;
    return d;
}

void EnsureRegistered() {
    static std::once_flag once;
    std::call_once(once, [] {
        detail::RegisterDeviceProcs();
        detail::RegisterMemoryProcs();
        detail::RegisterQueueProcs();
        detail::RegisterCommandProcs();
        detail::RegisterStateProcs();
        detail::RegisterWindowProcs();
        detail::RegisterTextureProcs();
    });
}

// The trap pool: thunk N reports the name bound to slot N. 1024 slots, twice the 523 names the game
// asks for; should a run ask for more, the extra names share the last slot, whose report says so.
constexpr std::size_t kTraps = 1024;
std::array<std::string, kTraps> g_trap_names;
std::array<bool, kTraps> g_trap_called{};
std::vector<std::string> g_called_order;
std::map<std::string, std::size_t> g_trap_of;

std::uint64_t TrapReport(std::size_t slot) {
    std::lock_guard<std::recursive_mutex> lock(Lock());
    if (!g_trap_called[slot]) {
        g_trap_called[slot] = true;
        g_called_order.push_back(g_trap_names[slot]);
        std::fprintf(stderr, "nvn shim: %s is not implemented (returns 0)\n", g_trap_names[slot].c_str());
    }
    return 0;
}

template <std::size_t N>
std::uint64_t Trap() {
    return TrapReport(N);
}

template <std::size_t... N>
constexpr std::array<std::uint64_t (*)(), sizeof...(N)> MakeTraps(std::index_sequence<N...>) {
    return {&Trap<N>...};
}

const std::array<std::uint64_t (*)(), kTraps> kTrapFns = MakeTraps(std::make_index_sequence<kTraps>{});

}  // namespace

bool RegisterProc(const char* name, Proc proc) {
    std::lock_guard<std::recursive_mutex> lock(Lock());
    if (!Table().emplace(name, proc).second)
        Duplicates().push_back(name);
    return true;
}

Proc GetProc(const char* name) {
    if (!name)
        return nullptr;
    EnsureRegistered();
    std::lock_guard<std::recursive_mutex> lock(Lock());
    auto it = Table().find(name);
    if (it != Table().end())
        return it->second;
    auto t = g_trap_of.find(name);
    if (t != g_trap_of.end())
        return reinterpret_cast<Proc>(kTrapFns[t->second]);
    std::size_t slot = g_trap_of.size();
    if (slot >= kTraps - 1) {
        slot = kTraps - 1;
        g_trap_names[slot] = "one of the NVN functions past the shim's 1023 trap slots";
    } else {
        g_trap_names[slot] = name;
    }
    g_trap_of.emplace(name, slot);
    return reinterpret_cast<Proc>(kTrapFns[slot]);
}

std::vector<std::string> Registered() {
    EnsureRegistered();
    std::lock_guard<std::recursive_mutex> lock(Lock());
    std::vector<std::string> out;
    for (const auto& kv : Table())
        out.push_back(kv.first);
    return out;
}

std::vector<std::string> TrapsCalled() {
    std::lock_guard<std::recursive_mutex> lock(Lock());
    return g_called_order;
}

std::vector<std::string> DuplicateRegistrations() {
    EnsureRegistered();
    std::lock_guard<std::recursive_mutex> lock(Lock());
    return Duplicates();
}

}  // namespace dread::nvn

// The bootstrap loader: the game's only import from NVN. fn_71001587f0 asks it for
// nvnDeviceGetProcAddress and nvnDeviceInitialize; any name resolves as GetProc does.
extern "C" void* nvnBootstrapLoader(const char* name) {
    return reinterpret_cast<void*>(dread::nvn::GetProc(name));
}
