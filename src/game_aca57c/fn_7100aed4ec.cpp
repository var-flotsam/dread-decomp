// Two copies of the move constructor of an iterator value (base::UnkIterValue's layout) over a
// map from string ids to arrays with a second pointer array alongside: the key is taken over
// (the source is left holding the empty id) and the array's storage is taken over.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkMovedStrId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkPairedArray {
    void* data;
    void** field_0x8;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkPairedArray(UnkPairedArray&& other)
        : data(other.data), field_0x8(other.field_0x8), count(other.count), capacity(other.capacity),
          step(other.step), flags(other.flags), allocator(other.allocator) {
        other.count = 0;
        other.data = nullptr;
        other.field_0x8 = nullptr;
    }
};

using UnkPairedArrayValue = base::UnkIterValue<UnkPairedArray, UnkMovedStrId>;

// (base::UnkIterValue's move constructor, inlined.)
__attribute__((always_inline)) static inline void Move(UnkPairedArrayValue* self, UnkPairedArrayValue* other) {
    using Pair = UnkPairedArrayValue::Value;
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) Pair(static_cast<Pair&&>(other->value));
    } else if (other->state == 2) {
        self->state = 1;
        UnkPairedArrayValue::Ref r = other->ref;
        new (&self->value) Pair{static_cast<UnkMovedStrId&&>(*const_cast<UnkMovedStrId*>(r.index)),
                                static_cast<UnkPairedArray&&>(*r.element)};
    }
}

// NON_MATCHING(minor): the two paths share the allocator's load (through a merged address)
// instead of each loading it.
void fn_7100aed4ec(UnkPairedArrayValue* self, UnkPairedArrayValue* other) { Move(self, other); }
// NON_MATCHING(minor): as fn_7100aed4ec.
void fn_7100e574c8(UnkPairedArrayValue* self, UnkPairedArrayValue* other) { Move(self, other); }
