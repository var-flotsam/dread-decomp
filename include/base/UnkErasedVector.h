#pragma once

// The engine array (base::UnkVector's layout) of 16-byte type-erased values: a storage word and
// the value's function table, whose copy, move and destroy entries the array uses. Neither name
// is known; the layout is what fn_710023b29c and its callers read. The table pointer is a union
// member: the original reloads the array's fields after storing it. UnkErasedVector itself has
// no constructor or destructor (fn_71007abba4 builds one field by field); the owning form the
// callback sets hold is UnkOwnedErasedVector.

#include "base/UnkVector.h"
#include "types.h"

struct UnkErasedValue_0x10;

// Function table of a type-erased value; only the slots used here are named.
struct UnkErasedOps {
    void* field_0x0[6];
    void (*copy)(UnkErasedValue_0x10* dst, const UnkErasedValue_0x10* src);
    void (*move)(UnkErasedValue_0x10* dst, UnkErasedValue_0x10* src);
    void (*destroy)(UnkErasedValue_0x10* self);
};

struct UnkErasedValue_0x10 {
    void* storage;
    union {
        UnkErasedOps* ops;
    };

    UnkErasedValue_0x10(UnkErasedValue_0x10&& other) {
        ops = other.ops;
        if (ops && ops->move)
            ops->move(this, &other);
        other.ops = nullptr;
    }
    ~UnkErasedValue_0x10() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};
static_assert(sizeof(UnkErasedValue_0x10) == 0x10, "UnkErasedValue_0x10 layout");

struct UnkErasedVector {
    UnkErasedValue_0x10* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
static_assert(sizeof(UnkErasedVector) == 0x28, "UnkErasedVector layout");

// The array as a callback set owns it: created empty with flags 0xf, destroyed with its values
// (the destructor's loop is written out; the element destructor inlined gives the same code).
struct UnkOwnedErasedVector : UnkErasedVector {
    UnkOwnedErasedVector() {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = 10;
        field_0x14 = true;
        flags = 0xf;
        allocator = fn_7100083990();
    }
    ~UnkOwnedErasedVector() {
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                UnkErasedValue_0x10& v = data[i];
                if (v.ops) {
                    v.ops->destroy(&v);
                    v.ops = nullptr;
                }
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// Destroys the values and empties the array, keeping its buffer (src/base/UnkErasedVector.cpp).
void fn_7100ad3588(UnkErasedVector* self);

// Appends a copy of `value` (not matched yet; fn_710023b29c calls it).
void fn_710023b424(UnkErasedVector* self, const UnkErasedValue_0x10* value);

// Copy-assignment (src/game_lo/fn_710023b29c.cpp): destroys the current elements, grows the
// buffer to the other array's capacity if needed, then appends a copy of each element. Returns
// self.
UnkErasedVector* fn_710023b29c(UnkErasedVector* self, const UnkErasedVector* other);
