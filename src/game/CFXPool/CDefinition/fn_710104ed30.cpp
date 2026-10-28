// Copies of fn_7100eaf85c (src/game_aca57c/fn_7100ad17ec.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_710104ed30, fn_710104f8e0.
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

// Free of an array (base::UnkVector's layout) of file-path ids (CFilePathStrId's pool) paired with a word: each id is
// released (and left as the empty id) before the array is freed.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkEmptiedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkEmptiedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
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

void fn_710104ed30(base::UnkVector<UnkIdWordPair>* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkIdWordPair();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}

void fn_710104f8e0(base::UnkVector<UnkIdWordPair>* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkIdWordPair();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}

