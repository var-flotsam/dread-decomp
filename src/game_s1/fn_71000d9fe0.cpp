// Copies of fn_7100e7224c and fn_7100e7232c (src/game_aca57c/fn_7100e4ee60.cpp) for a table of records holding only a
// string id, naming their own callees and data (the comments are adapted from the original's): fn_71000d9fe0,
// fn_71000da0c0. The table's Begin and End, which point an iterator value at the first key and value or one past the
// last.
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdRecord8 {
    UnkReleasedStrId id;
};

template <class Value>
struct UnkIdRecordTable {
    Value* values;
    UnkReleasedStrId* keys;
    u32 count;
};

template <class Value>
__attribute__((always_inline)) static inline void SetRef(base::UnkIterValue<Value, UnkReleasedStrId>* out,
                                                         const UnkReleasedStrId* index, Value* element) {
    // base::UnkIterValue::SetRef, inlined.
    using Pair = typename base::UnkIterValue<Value, UnkReleasedStrId>::Value;
    if (out->state == 2) {
        out->state = 0;
    } else if (out->state == 1) {
        out->value.~Pair();
        out->state = 0;
    }
    out->state = 2;
    out->ref.index = index;
    out->ref.element = element;
}

template <class Value>
__attribute__((always_inline)) static inline void Begin(const UnkIdRecordTable<Value>* self,
                                                        base::UnkIterValue<Value, UnkReleasedStrId>* out) {
    SetRef(out, self->keys, self->values);
}

template <class Value>
__attribute__((always_inline)) static inline void End(const UnkIdRecordTable<Value>* self,
                                                      base::UnkIterValue<Value, UnkReleasedStrId>* out) {
    // (the element is addressed first here: with 8-byte values the two sums would otherwise swap registers)
    Value* element = self->values + self->count;
    SetRef(out, self->keys + self->count, element);
}

void fn_71000d9fe0(const UnkIdRecordTable<UnkIdRecord8>* self, base::UnkIterValue<UnkIdRecord8, UnkReleasedStrId>* out) {
    Begin(self, out);
}
void fn_71000da0c0(const UnkIdRecordTable<UnkIdRecord8>* self, base::UnkIterValue<UnkIdRecord8, UnkReleasedStrId>* out) {
    End(self, out);
}
