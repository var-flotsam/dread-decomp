// nn::account for the port: one local user, always open, with a fixed non-zero id, so that the
// save system mounts the same save directory every time. Every stub names the boot-path function
// that depends on it in a TODO.

#include "nn/account.h"

#include <cstring>

namespace nn::account {

namespace {

// Written into the first word of the game's handle; any non-zero value would do.
constexpr u64 kLocalUserHandle = 0x445255534552;  // 'DRUSER'
constexpr Uid kLocalUser{{0x1, 0x0}};

}  // namespace

void Initialize() {
    // TODO(M1): fn_71001325d0.
}

bool TryOpenPreselectedUser(UserHandle* out) {
    // TODO(M1): fn_71001325d0 reads the user id only if this succeeds.
    std::memcpy(out, &kLocalUserHandle, sizeof(kLocalUserHandle));
    return true;
}

Result GetUserId(Uid* out, const UserHandle&) {
    // TODO(M1): fn_71001325d0 stores the id in a global, which the save system passes to
    // nn::fs::EnsureSaveData and MountSaveData.
    *out = kLocalUser;
    return ResultSuccess();
}

void CloseUser(const UserHandle&) {
    // TODO(M1): the shutdown fn_7100132730, when the id is not zero.
}

}  // namespace nn::account
