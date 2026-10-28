// Copies of fn_7100e59270 (src/base/reflection/fn_7100adae44.cpp) for other types, with their own
// callees and data: fn_7100f78e68.
// Assignment from an in-place (state 1) or by-reference (state 2) pair of an id and a record: the
// id is moved (released, taken, the other's left empty) and the record assigned out of line.
#include "base/global/CStrId.h"
#include "types.h"

template <class X, class Y>
struct UnkValuePairRef {
    union {
        struct {
            X x;
            Y y;
        } values;
        struct {
            X* x;
            Y* y;
        } refs;
    };
    s32 state;

    X& GetX() { return state == 2 ? *refs.x : values.x; }
    Y& GetY() { return state == 2 ? *refs.y : values.y; }
};

struct UnkPairReleasedStrId {
    base::global::UnkStringData* data;

    ~UnkPairReleasedStrId() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

// The id pointers are moved bitwise (the original reloads the pair's state after them).
__attribute__((always_inline)) static inline void MoveAssign(UnkPairReleasedStrId* self, UnkPairReleasedStrId* other) {
    self->~UnkPairReleasedStrId();
    __builtin_memcpy(&self->data, &other->data, sizeof(self->data));
    base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
    __builtin_memcpy(&other->data, &empty, sizeof(empty));
}

template <class Y>
__attribute__((always_inline)) static inline UnkValuePairRef<UnkPairReleasedStrId, Y>* MoveAssignSet(
    UnkValuePairRef<UnkPairReleasedStrId, Y>* self, UnkValuePairRef<UnkPairReleasedStrId, Y>* other) {
    if (other->state == 1) {
        MoveAssign(&self->GetX(), &other->values.x);
        self->GetY() = other->values.y;
    } else if (other->state == 2) {
        UnkPairReleasedStrId* x = other->refs.x;
        MoveAssign(&self->GetX(), x);
        self->GetY() = *other->refs.y;
    }
    return self;
}

#define UNK_ASSIGNED_RECORD(name, size, assign)                                      \
    struct name {                                                                    \
        u64 unk_0x0[size / 8];                                                       \
        name& operator=(const name& other);                                          \
    };                                                                               \
    void assign(name* self, const name* other);                                      \
    __attribute__((always_inline)) inline name& name::operator=(const name& other) { \
        assign(this, &other);                                                        \
        return *this;                                                                \
    }

UNK_ASSIGNED_RECORD(UnkAssignedRecord_f78e68, 0x3a8, fn_7100f477a0)

using UnkIdRecordPair_f78e68 = UnkValuePairRef<UnkPairReleasedStrId, UnkAssignedRecord_f78e68>;

UnkIdRecordPair_f78e68* fn_7100f78e68(UnkIdRecordPair_f78e68* self, UnkIdRecordPair_f78e68* other) {
    return MoveAssignSet(self, other);
}
