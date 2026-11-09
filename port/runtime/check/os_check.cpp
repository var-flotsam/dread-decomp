// os_check: the port's nn::os events and multi-wait holders through the sequence the movie worker
// and the main thread run on the title (src/os.cpp, "finalised the console's way"): a stream's
// event linked into a multi-wait, finalised (while still linked) and initialised again in either
// order, then linked, signalled and waited on. No game data; exits non-zero on the first failure.
//   os_check

#include <cstdio>

#include "nn/os.h"

namespace {

int g_failed = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "FAIL: %s\n", what);
        ++g_failed;
    }
}

struct alignas(8) Storage {
    unsigned char bytes[0x40];
};

}  // namespace

int main() {
    Storage multi_storage{}, holder_storage{};
    nn::os::EventType event{};
    auto* multi = reinterpret_cast<nn::os::MultiWaitType*>(&multi_storage);
    auto* holder = reinterpret_cast<nn::os::MultiWaitHolderType*>(&holder_storage);
    nn::os::InitializeMultiWait(multi);

    // The first use: initialised, linked, signalled, waited on.
    nn::os::InitializeEvent(&event, false, nn::os::EventClearMode_AutoClear);
    nn::os::InitializeMultiWaitHolder(holder, &event);
    nn::os::SetMultiWaitHolderUserData(holder, 7);
    nn::os::LinkMultiWaitHolder(multi, holder);
    nn::os::SignalEvent(&event);
    Check(nn::os::WaitAny(multi) == holder, "the first use: the signalled holder");

    // The main thread initialises the stream again before the worker's "off" entry runs.
    nn::os::InitializeEvent(&event, false, nn::os::EventClearMode_AutoClear);
    nn::os::InitializeMultiWaitHolder(holder, &event);  // still linked: unlinked first
    nn::os::SetMultiWaitHolderUserData(holder, 8);
    // The worker's "off" entry: unlink, finalise the holder and the event.
    nn::os::UnlinkMultiWaitHolder(holder);
    nn::os::FinalizeMultiWaitHolder(holder);
    nn::os::FinalizeEvent(&event);
    // Its "on" entry: link the (re-initialised) holder; the stream signals.
    nn::os::LinkMultiWaitHolder(multi, holder);
    nn::os::SignalEvent(&event);
    Check(nn::os::WaitAny(multi) == holder, "after finalise-after-initialise: the holder still wakes the wait");
    Check(nn::os::GetMultiWaitHolderUserData(holder) == 8, "the user data of the second initialisation");

    // A holder finalised while linked is unlinked, and can be initialised and linked again.
    nn::os::FinalizeMultiWaitHolder(holder);
    nn::os::InitializeMultiWaitHolder(holder, &event);
    nn::os::LinkMultiWaitHolder(multi, holder);
    nn::os::SignalEvent(&event);
    Check(nn::os::WaitAny(multi) == holder, "finalised while linked, then initialised and linked again");
    Check(!nn::os::TryWaitEvent(&event), "the auto-clear signal was consumed by the wait");

    // The main thread built the stream again (its events and holders zeroed) before the worker's
    // "off" entry ran: unlinking and finalising them is a no-op, and they initialise as new.
    nn::os::UnlinkMultiWaitHolder(holder);
    nn::os::FinalizeMultiWaitHolder(holder);
    nn::os::FinalizeEvent(&event);
    Storage zeroed_holder{};
    nn::os::EventType zeroed_event{};
    auto* fresh = reinterpret_cast<nn::os::MultiWaitHolderType*>(&zeroed_holder);
    nn::os::UnlinkMultiWaitHolder(fresh);
    nn::os::FinalizeMultiWaitHolder(fresh);
    nn::os::FinalizeEvent(&zeroed_event);
    nn::os::InitializeEvent(&zeroed_event, false, nn::os::EventClearMode_AutoClear);
    nn::os::InitializeMultiWaitHolder(fresh, &zeroed_event);
    nn::os::LinkMultiWaitHolder(multi, fresh);
    nn::os::SignalEvent(&zeroed_event);
    Check(nn::os::WaitAny(multi) == fresh, "zeroed objects: unlink and finalise do nothing, then they work");

    if (g_failed) {
        std::fprintf(stderr, "os_check: %d failed\n", g_failed);
        return 1;
    }
    std::printf("os_check: all passed\n");
    return 0;
}
