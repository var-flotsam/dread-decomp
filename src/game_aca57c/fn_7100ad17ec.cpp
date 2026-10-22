// Equality of two iterator values over 0x80-byte records keyed by a word (base::UnkIterValue's
// layout, the state at 0x88): the keys are compared, then the records by their word at 8.
#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkRecord80 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10[14];
};

using UnkRecord80IterValue = base::UnkIterValue<UnkRecord80, u32>;

// (base::operator== for iterator values, inlined.)
__attribute__((always_inline)) static inline bool Equal(const UnkRecord80IterValue* a, const UnkRecord80IterValue* b) {
    u32 ka = a->state == 2 ? *a->ref.index : a->value.index;
    u32 kb = b->state == 2 ? *b->ref.index : b->value.index;
    if (ka != kb)
        return false;
    const UnkRecord80* x = a->state == 2 ? a->ref.element : &a->value.element;
    const UnkRecord80* y = b->state == 2 ? b->ref.element : &b->value.element;
    return x->field_0x8 == y->field_0x8;
}

bool fn_7100ad17ec(const UnkRecord80IterValue* a, const UnkRecord80IterValue* b) { return Equal(a, b); }
bool fn_7100ad1af4(const UnkRecord80IterValue* a, const UnkRecord80IterValue* b) { return Equal(a, b); }

// Free of an array (base::UnkVector's layout) of string ids paired with a word: each id is
// released (and left as the empty id) before the array is freed.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkEmptiedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkEmptiedStrId() {
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

struct UnkIdWordPair {
    UnkEmptiedStrId id;
    u64 field_0x8;
};

void fn_7100eaf85c(base::UnkVector<UnkIdWordPair>* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkIdWordPair();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}
