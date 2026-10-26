// Copies of fn_7100d86fe8, fn_7100e71cc4 (src/game_aca57c/fn_7100e4ee60.cpp), which the binary
// repeats at other addresses: fn_71002520fc, fn_71000df8a0, fn_7100916de8, fn_710104b400, fn_71010c6e30, fn_710079ce08, fn_71000a57f8.
// Tables' Clear: the string ids in the key array (at 8) are released, then the values in the
// value array (at 0) are destroyed (records with an id at 0x18 of 0x28 bytes or at 0 of 0x18
// bytes, or owned objects that are deleted), and the count (at 0x10) is zeroed. The arrays
// themselves are kept.
// Also the tables' Begin and End, which point an iterator value at the first key and value or
// one past the last.
#include "base/UnkCollectionIter.h"
#include "base/UnkFreedVectorPair.h"
#include "base/UnkKeyedStrIds.h"
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
template <class T, void (*Destroy)(T*)>
struct UnkOwned {
    T* ptr;

    __attribute__((always_inline)) ~UnkOwned() {
        T* p = ptr;
        ptr = nullptr;
        if (p) {
            Destroy(p);
            operator delete(p);
        }
    }
};

void fn_7100aef674(void* self);

// NON_MATCHING(minor): the value array and the count swap registers in the second loop, whose
// compare takes the count already widened.
void fn_7100ae31f8(UnkIdRecordTable<UnkOwned<UnkKeyedStrIds, fn_7100ae2df8>>* self);
// NON_MATCHING(minor): as fn_7100ae31f8.

void fn_71000a57f8(UnkIdRecordTable<UnkIdRecord18>* self) { Clear(self); }

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

// Tables whose 0x28-byte values have an out-of-line destructor.
template <class T, void (*Destroy)(T*)>
struct UnkOutOfLineValue28 : T {
    ~UnkOutOfLineValue28() { Destroy(this); }
};

using UnkValue28A = UnkOutOfLineValue28<UnkKeyedStrIds, fn_71000df388>;
using UnkValue28B = UnkOutOfLineValue28<UnkSecondPtrArray<UnkFreedVectorPair>, fn_7100e493f4>;

// A table of 0x60-byte records holding two arrays (base::UnkVector's layout, freed when
// destroyed) and four words.
struct UnkFreedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkFreedArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkArrayRecord60 {
    UnkFreedArray field_0x0;
    UnkFreedArray field_0x28;
    u32 field_0x50[4];
};

void fn_7100e572d8(const UnkIdRecordTable<UnkArrayRecord60>* self,
                   base::UnkIterValue<UnkArrayRecord60, UnkReleasedStrId>* out);
void fn_7100e573b0(const UnkIdRecordTable<UnkArrayRecord60>* self,
                   base::UnkIterValue<UnkArrayRecord60, UnkReleasedStrId>* out);

// A table of words.
void fn_71002520fc(const UnkIdRecordTable<u64>* self, base::UnkIterValue<u64, UnkReleasedStrId>* out) {
    Begin(self, out);
}

void fn_71000df8a0(const UnkIdRecordTable<u64>* self, base::UnkIterValue<u64, UnkReleasedStrId>* out) {
    Begin(self, out);
}

void fn_7100916de8(const UnkIdRecordTable<u64>* self, base::UnkIterValue<u64, UnkReleasedStrId>* out) {
    Begin(self, out);
}

void fn_710104b400(const UnkIdRecordTable<u64>* self, base::UnkIterValue<u64, UnkReleasedStrId>* out) {
    Begin(self, out);
}

void fn_71010c6e30(const UnkIdRecordTable<u64>* self, base::UnkIterValue<u64, UnkReleasedStrId>* out) {
    Begin(self, out);
}

void fn_710079ce08(const UnkIdRecordTable<u64>* self, base::UnkIterValue<u64, UnkReleasedStrId>* out) {
    Begin(self, out);
}

// NON_MATCHING(minor): the two end pointers swap registers.

// Destructor of a keyed record of three arrays.
struct UnkKeyedThreeArrays {
    UnkReleasedStrId key;
    UnkFreedArray field_0x8;
    u64 field_0x30[4];
    UnkFreedArray field_0x50;
    UnkFreedArray field_0x78;
};

// An iterator value over the 0x60-byte records taking another's reference: its owned pair is
// destroyed, then the state and the reference are copied.
using UnkArrayRecord60IterValue = base::UnkIterValue<UnkArrayRecord60, UnkReleasedStrId>;

__attribute__((always_inline)) static inline void TakeRef(UnkArrayRecord60IterValue* self,
                                                          const UnkArrayRecord60IterValue* other) {
    using Pair = UnkArrayRecord60IterValue::Value;
    if (self->state == 2) {
        self->state = 0;
    } else if (self->state == 1) {
        self->value.~Pair();
        self->state = 0;
    }
    self->state = other->state;
    self->ref = other->ref;
}

