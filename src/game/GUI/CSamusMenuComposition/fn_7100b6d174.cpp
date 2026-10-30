// Destructor of three string ids (released in reverse order). Three identical copies.
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkStrIdTriple {
    base::UnkStrIdMember id_0x0;
    base::UnkStrIdMember id_0x8;
    base::UnkStrIdMember id_0x10;

    __attribute__((always_inline)) ~UnkStrIdTriple() {}
};

void fn_7100b6d174(UnkStrIdTriple* self) { self->~UnkStrIdTriple(); }
void fn_7100c896a4(UnkStrIdTriple* self) { self->~UnkStrIdTriple(); }
void fn_7100c89ac0(UnkStrIdTriple* self) { self->~UnkStrIdTriple(); }
// Copies in two other units, on the same three ids further into the object.
template <u32 At>
struct UnkStrIdTripleAt {
    u8 unk_0x0[At];
    UnkStrIdTriple ids;

    __attribute__((always_inline)) ~UnkStrIdTripleAt() {}
};

void fn_7100431cec(UnkStrIdTripleAt<0x8>* self) { self->~UnkStrIdTripleAt(); }
void fn_7101171280(UnkStrIdTripleAt<0x68>* self) { self->~UnkStrIdTripleAt(); }
