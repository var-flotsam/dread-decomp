// Copies of fn_7100c896a4 (src/game_aca57c/fn_7100b6d174.cpp), which the binary
// repeats at other addresses: fn_7100497e30, fn_7100726674.
// Destructor of three string ids (released in reverse order). Three identical copies.
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkStrIdTriple {
    base::UnkStrIdMember id_0x0;
    base::UnkStrIdMember id_0x8;
    base::UnkStrIdMember id_0x10;

    __attribute__((always_inline)) ~UnkStrIdTriple() {}
};

void fn_7100497e30(UnkStrIdTriple* self) { self->~UnkStrIdTriple(); }

void fn_7100726674(UnkStrIdTriple* self) { self->~UnkStrIdTriple(); }

