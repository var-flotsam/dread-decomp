// Copies of fn_7100e604a8 (src/game_aca57c/fn_7100adae44.cpp) for another record type, naming their own callees and
// data (the comments are adapted from the original's): fn_71000d6310, fn_71000d73b8. Move-assignment of a pair of an id
// and a record held either in place or by reference (state 2), from another such pair: the id is moved (released,
// taken, the other's left empty) and the record assigned out of line (fn_71000a1ba4).
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

struct UnkAssignedRecord_a1ba4 {
    u64 unk_0x0[0x18 / 8];
    UnkAssignedRecord_a1ba4& operator=(const UnkAssignedRecord_a1ba4& other);
};
void fn_71000a1ba4(UnkAssignedRecord_a1ba4* self, const UnkAssignedRecord_a1ba4* other);
__attribute__((always_inline)) inline UnkAssignedRecord_a1ba4& UnkAssignedRecord_a1ba4::operator=(
    const UnkAssignedRecord_a1ba4& other) {
    fn_71000a1ba4(this, &other);
    return *this;
}

using UnkIdRecordPair = UnkValuePairRef<UnkPairReleasedStrId, UnkAssignedRecord_a1ba4>;

UnkIdRecordPair* fn_71000d6310(UnkIdRecordPair* self, UnkIdRecordPair* other) { return MoveAssignSet(self, other); }
UnkIdRecordPair* fn_71000d73b8(UnkIdRecordPair* self, UnkIdRecordPair* other) { return MoveAssignSet(self, other); }
