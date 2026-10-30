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

// A node of a tree whose children are listed at +0x70 (src/game_s7/fn_7101072a80.cpp sorts by the
// depth below).
struct UnkObj_7101072a80 {
    u8 pad_0x0[0x70];
    UnkObj_7101072a80** children;
    u32 child_count;
};

// The depth of the tree below `obj`, counting it: 1 for a leaf.
u32 fn_710019536c(UnkObj_7101072a80* obj) {
    u32 depth = 0;
    for (UnkObj_7101072a80 **it = obj->children, **end = it + obj->child_count; it != end; ++it) {
        u32 d = fn_710019536c(*it);
        if (d > depth)
            depth = d;
    }
    return depth + 1;
}
