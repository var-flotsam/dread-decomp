#pragma once

// nn::account: the console user whose save data the game opens. The boot path calls Initialize
// and TryOpenPreselectedUser, reads the user id (GetUserId) to mount the save, then CloseUser.
// On PC: one fixed local user, so the save directory is always the same.
//
// `main` imports (data/functions/): Initialize, TryOpenPreselectedUser, GetUserId, CloseUser.
// Declared so far: the user id, which nn::fs's save calls take (fn_7100fc3510). The calls come
// with their first caller.

#include "nn/nn_common.h"

namespace nn::account {

// A user id: two words. fn_7100132730 tests both for zero before closing the user.
struct Uid {
    u64 value[2];
};
static_assert(sizeof(Uid) == 16, "two words in the game's globals");

}  // namespace nn::account
