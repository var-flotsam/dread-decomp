#pragma once

// nn::account: the console user whose save data the game opens. The system start-up
// fn_71001325d0 calls Initialize and TryOpenPreselectedUser and, if a user is open, reads its id
// (GetUserId) into a global; the save system (fn_7100fc3510) mounts that user's save; the shutdown
// fn_7100132730 closes the user if the id is not zero. On PC: one fixed local user, so the save
// directory is always the same.
//
// `main` imports (data/functions/): Initialize, TryOpenPreselectedUser, GetUserId, CloseUser.
// All declared below.

#include "nn/nn_common.h"

namespace nn::account {

// A user id: two words. fn_7100132730 tests both for zero before closing the user.
struct Uid {
    u64 value[2];
};
static_assert(sizeof(Uid) == 16, "two words in the game's globals");

// An open user, in a game global (fn_71001325d0 passes its address to TryOpenPreselectedUser,
// GetUserId and CloseUser). Its size is not known from the game; the port writes and reads its
// first word only, which any handle has.
struct UserHandle;

void Initialize();
bool TryOpenPreselectedUser(UserHandle* out);
Result GetUserId(Uid* out, const UserHandle& user);
void CloseUser(const UserHandle& user);

}  // namespace nn::account
