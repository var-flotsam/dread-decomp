// Copies of fn_7100316018 (src/game_s2/fn_7100316018.cpp), which the binary
// repeats at other addresses: fn_710082efe8, fn_71006f552c, fn_71006f5630.
// Makes the slot `out` refer to the weak handle of `self` (created on first use), releasing what it
// referred to before.
#include "game_s2/UnkWeakHandle.h"

void fn_710082efe8(UnkWeakTarget* self, UnkWeakHandle** out) {
    AssignRef(out, GetWeakHandle(self));
}

void fn_71006f552c(UnkWeakTarget* self, UnkWeakHandle** out) {
    AssignRef(out, GetWeakHandle(self));
}

void fn_71006f5630(UnkWeakTarget* self, UnkWeakHandle** out) {
    AssignRef(out, GetWeakHandle(self));
}

