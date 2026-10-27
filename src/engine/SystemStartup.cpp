// The system start-up and shutdown that nnMain's main loop (fn_7100132d30) and its shutdown
// (fn_7100132e90) run around the game. fn_71001325d0 initialises the SDK heap over 32 MB of the
// .bss and hands it to the graphics driver with 8 MB of memory, mounts the RomFS as "rom" over a
// cache from the engine's allocator (aborting if the mount fails), opens the preselected user and
// keeps its id, makes a session UUID, and sets up nn::oe (both change notifications, the
// performance configuration, the operation mode) and nn::time. fn_7100132730 undoes it in reverse.
//
// The SDK calls are declared here in the form the imports carry (their mangled names): lib/nnsdk
// lacks several of them and the port's runtime defines the same names (port/runtime/include/nn/).
// Results are taken as s32, which is how they come back in w0.

#include "base/UnkVector.h"
#include "types.h"

namespace nn {
class Result;

namespace account {
struct Uid;
struct UserHandle;
void Initialize();
bool TryOpenPreselectedUser(UserHandle* out);
s32 GetUserId(Uid* out, const UserHandle& user);
void CloseUser(const UserHandle& user);
}  // namespace account

namespace diag::detail {
[[noreturn]] void AbortImpl(const char* condition, const char* function, const char* file, s32 line,
                            const Result* result, const char* format, ...);
}  // namespace diag::detail

namespace fs {
void SetAllocator(void* (*allocate)(u64 size), void (*deallocate)(void* pointer, u64 size));
s32 QueryMountRomCacheSize(u64* out_size);
s32 MountRom(const char* name, void* cache, u64 cache_size);
void Unmount(const char* name);
}  // namespace fs

namespace mem {
class StandardAllocator {
public:
    void Initialize(void* address, u64 size);
    void Finalize();
};
}  // namespace mem

namespace oe {
enum PerformanceMode : s32 {};
void Initialize();
void SetOperationModeChangedNotificationEnabled(bool enabled);
void SetPerformanceModeChangedNotificationEnabled(bool enabled);
void SetPerformanceConfiguration(PerformanceMode mode, s32 configuration);
s32 GetOperationMode();
void EnterExitRequestHandlingSection();
void LeaveExitRequestHandlingSection();
}  // namespace oe

namespace time {
void Initialize();
void Finalize();
}  // namespace time

namespace util {
struct Uuid {
    u64 words[2];
};
Uuid GenerateUuid();
}  // namespace util
}  // namespace nn

namespace nv {
void SetGraphicsAllocator(void* (*allocate)(u64 size, u64 alignment, void* user),
                          void (*free)(void* pointer, void* user),
                          void* (*reallocate)(void* pointer, u64 size, void* user), void* user);
void InitializeGraphics(void* memory, u64 size);
void FinalizeGraphics();
}  // namespace nv

// The SDK heap (constructed by the unit's initialiser fn_7100132b80) and what it manages.
extern nn::mem::StandardAllocator g_unk_710ab70000 __attribute__((visibility("hidden")));
extern u8 g_unk_7108b70000[];  // the heap's 32 MB
extern u8 g_unk_710ab71000[];  // the graphics driver's 8 MB
extern u64 g_unk_710b371018;   // the RomFS cache's size
extern void* g_unk_710b371020;  // the RomFS cache
extern nn::account::UserHandle g_unk_710b371040;
extern u64 g_unk_710b371030[2];  // the user id
extern nn::util::Uuid g_unk_710b371058;  // the session id
extern u8 g_unk_710b37106c;
extern s32 g_unk_710b371068;  // the operation mode (refreshed by fn_71001327c0)

// The graphics allocator's callbacks (the heap as `user`) and nn::fs's allocator: this unit's own
// (their addresses are taken PC-relatively, not through the GOT).
void* fn_71001322b0(u64 size, u64 alignment, void* user) __attribute__((visibility("hidden")));
void fn_71001322d0(void* pointer, void* user) __attribute__((visibility("hidden")));
void* fn_71001322e0(void* pointer, u64 size, void* user) __attribute__((visibility("hidden")));
void* fn_7100132b20(u64 size) __attribute__((visibility("hidden")));
void fn_7100132b54(void* pointer, u64 size) __attribute__((visibility("hidden")));

void fn_71001325d0() {
    g_unk_710ab70000.Initialize(g_unk_7108b70000, 0x2000000);
    nv::SetGraphicsAllocator(fn_71001322b0, fn_71001322d0, fn_71001322e0, &g_unk_710ab70000);
    nv::InitializeGraphics(g_unk_710ab71000, 0x800000);
    nn::fs::SetAllocator(fn_7100132b20, fn_7100132b54);
    nn::fs::QueryMountRomCacheSize(&g_unk_710b371018);
    void* cache = fn_7100083ba0(g_unk_710b371018, 0x16);
    g_unk_710b371020 = cache;
    if (cache) {
        alignas(8) s32 result = nn::fs::MountRom("rom", cache, g_unk_710b371018);
        if (result != 0)
            nn::diag::detail::AbortImpl("", "", "", 0, reinterpret_cast<const nn::Result*>(&result), "");
    }
    nn::oe::EnterExitRequestHandlingSection();
    nn::account::Initialize();
    if (nn::account::TryOpenPreselectedUser(&g_unk_710b371040))
        nn::account::GetUserId(reinterpret_cast<nn::account::Uid*>(g_unk_710b371030), g_unk_710b371040);
    g_unk_710b371058 = nn::util::GenerateUuid();
    nn::oe::Initialize();
    nn::oe::SetOperationModeChangedNotificationEnabled(true);
    nn::oe::SetPerformanceModeChangedNotificationEnabled(true);
    nn::oe::SetPerformanceConfiguration(nn::oe::PerformanceMode(0), 0x20003);
    g_unk_710b37106c = 0;
    g_unk_710b371068 = nn::oe::GetOperationMode();
    nn::time::Initialize();
}

void fn_7100132730() {
    nn::time::Finalize();
    if (g_unk_710b371030[0] || g_unk_710b371030[1]) {
        nn::account::CloseUser(g_unk_710b371040);
        g_unk_710b371030[0] = 0;
        g_unk_710b371030[1] = 0;
    }
    if (g_unk_710b371058.words[1] || g_unk_710b371058.words[0])
        g_unk_710b371058 = nn::util::Uuid{};
    nv::FinalizeGraphics();
    g_unk_710ab70000.Finalize();
    nn::oe::LeaveExitRequestHandlingSection();
    nn::fs::Unmount("rom");
}
