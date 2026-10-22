// Tables' Clear: the string ids in the key array (at 8) are released, then the values in the
// value array (at 0) are destroyed (records with an id at 0x18 of 0x28 bytes or at 0 of 0x18
// bytes, or owned objects that are deleted), and the count (at 0x10) is zeroed. The arrays
// themselves are kept.
// Also the tables' End, which points an iterator value at the key and value one past the last.
#include "base/UnkCollectionIter.h"
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

struct UnkIdRecord28 {
    u64 field_0x0[3];
    UnkReleasedStrId id;
    u64 field_0x20;
};

struct UnkIdRecord18 {
    UnkReleasedStrId id;
    u64 field_0x8[2];
};

template <class Value>
struct UnkIdRecordTable {
    Value* values;
    UnkReleasedStrId* keys;
    u32 count;
};

template <class Value>
__attribute__((always_inline)) static inline void Clear(UnkIdRecordTable<Value>* self) {
    if (self->keys && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkReleasedStrId();
    }
    if (self->values && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].~Value();
    }
    self->count = 0;
}

// An owning pointer: the slot is cleared before the object is destroyed and freed.
template <void (*Destroy)(void*)>
struct UnkOwned {
    void* ptr;

    __attribute__((always_inline)) ~UnkOwned() {
        void* p = ptr;
        ptr = nullptr;
        if (p) {
            Destroy(p);
            operator delete(p);
        }
    }
};

void fn_7100ae2df8(void* self);  // the owned object's destructor
void fn_7100aef674(void* self);

// NON_MATCHING(minor): the value array and the count swap registers in the second loop, whose
// compare takes the count already widened.
void fn_7100ae31f8(UnkIdRecordTable<UnkOwned<fn_7100ae2df8>>* self) { Clear(self); }
// NON_MATCHING(minor): as fn_7100ae31f8.
void fn_7100aecdc4(UnkIdRecordTable<UnkOwned<fn_7100aef674>>* self) { Clear(self); }

void fn_7100e4ee60(UnkIdRecordTable<UnkIdRecord28>* self) { Clear(self); }
void fn_7100e71cc4(UnkIdRecordTable<UnkIdRecord18>* self) { Clear(self); }

// Begin and End: the iterator value is pointed at the first key and value, or one past the last.
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
    SetRef(out, self->keys + self->count, self->values + self->count);
}

void fn_7100e5c46c(const UnkIdRecordTable<UnkIdRecord28>* self, base::UnkIterValue<UnkIdRecord28, UnkReleasedStrId>* out) {
    Begin(self, out);
}
void fn_7100e7224c(const UnkIdRecordTable<UnkIdRecord18>* self, base::UnkIterValue<UnkIdRecord18, UnkReleasedStrId>* out) {
    Begin(self, out);
}

void fn_7100e5c54c(const UnkIdRecordTable<UnkIdRecord28>* self, base::UnkIterValue<UnkIdRecord28, UnkReleasedStrId>* out) {
    End(self, out);
}
void fn_7100e7232c(const UnkIdRecordTable<UnkIdRecord18>* self, base::UnkIterValue<UnkIdRecord18, UnkReleasedStrId>* out) {
    End(self, out);
}
