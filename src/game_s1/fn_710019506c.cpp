// Destructor of a plain object: frees the blocks at 0x60 and 0x58, then its members, an array of
// words at 0x70 and a string id at 0x68. fn_710019e82c is a relative.

#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkObject_710019506c {
    u8 pad_0x0[0x58];
    void* block_0x58;
    void* block_0x60;
    base::UnkStrIdMember id;
    base::UnkVector<u32> values;

    __attribute__((always_inline)) ~UnkObject_710019506c() {
        if (block_0x60) {
            operator delete(block_0x60);
            block_0x60 = nullptr;
        }
        if (block_0x58) {
            operator delete(block_0x58);
            block_0x58 = nullptr;
        }
        values.Free();
    }
};

void fn_710019506c(UnkObject_710019506c* self) { self->~UnkObject_710019506c(); }
