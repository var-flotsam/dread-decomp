// Destructors (complete and deleting) of a vibration player (vtable unk_7101a1a3a8, derived from
// nn::hid::VibrationPlayer) with two node connections at 0xf0 and 0x128 and a mutex at 0x1b0
// (finalized by fn_7100066918): fn_71000707dc and fn_7100070828. Called only through the vtable.

#include <new>

#include "base/UnkDefaultAllocator.h"
#include "base/input/UnkHidVibration.h"
#include "types.h"

extern char unk_7101a1a3a8[];

// A mutex finalized on destruction.
struct UnkOwnedMutex_71000707dc {
    nn::os::MutexType mutex;

    ~UnkOwnedMutex_71000707dc() { fn_7100066918(&mutex); }
};

struct UnkVibrationPlayer_71000707dc : nn::hid::VibrationPlayer {
    nn::hid::VibrationNodeConnection connection_0xf0;
    nn::hid::VibrationNodeConnection connection_0x128;
    u8 pad_0x160[0x50];
    UnkOwnedMutex_71000707dc mutex;

    ~UnkVibrationPlayer_71000707dc() { vtable = unk_7101a1a3a8 + 0x10; }
};

void fn_71000707dc(UnkVibrationPlayer_71000707dc* self) { self->~UnkVibrationPlayer_71000707dc(); }

void fn_7100070828(UnkVibrationPlayer_71000707dc* self) {
    self->~UnkVibrationPlayer_71000707dc();
    operator delete(self);
}
