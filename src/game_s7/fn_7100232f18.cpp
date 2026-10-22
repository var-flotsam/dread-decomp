// Copies of fn_7100d78378 (src/game_aca57c/fn_7100d78378.cpp), which the binary
// repeats at other addresses: fn_7100232f18.
// An object's weak handle, created on first use, and a setter making the weak reference at 0x398
// refer to another object's handle.
#include "game_s2/UnkWeakHandle.h"

UnkWeakHandle* fn_7100232f18(UnkWeakTarget* self) { return GetWeakHandle(self); }

struct UnkWeakRef398 {
    u8 pad_0x0[0x398];
    UnkWeakHandle* field_0x398;
};

