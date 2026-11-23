// Copies of fn_7100e604a8 (src/base/reflection/fn_7100adae44.cpp) for another record type, naming their own callees and
// data (the comments are adapted from the original's): fn_71000d6310, fn_71000d73b8. Move-assignment of a pair of an id
// and a record held either in place or by reference (state 2), from another such pair: the id is moved (released,
// taken, the other's left empty) and the record assigned out of line (fn_71000a1ba4).
#include "base/UnkAssignedRecord_a1ba4.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "base/reflection.h"
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

// The id pointers are moved bitwise (the original reloads the pair's state after them).
__attribute__((always_inline)) static inline void MoveAssign(UnkStrIdTwoStep* self, UnkStrIdTwoStep* other) {
    self->~UnkStrIdTwoStep();
    self->data = other->data;
    base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
    other->data = empty;
}

template <class Y>
__attribute__((always_inline)) static inline UnkValuePairRef<UnkStrIdTwoStep, Y>* MoveAssignSet(
    UnkValuePairRef<UnkStrIdTwoStep, Y>* self, UnkValuePairRef<UnkStrIdTwoStep, Y>* other) {
    if (other->state == 1) {
        MoveAssign(&self->GetX(), &other->values.x);
        self->GetY() = other->values.y;
    } else if (other->state == 2) {
        UnkStrIdTwoStep* x = other->refs.x;
        MoveAssign(&self->GetX(), x);
        self->GetY() = *other->refs.y;
    }
    return self;
}

__attribute__((always_inline)) inline UnkAssignedRecord_a1ba4& UnkAssignedRecord_a1ba4::operator=(
    const UnkAssignedRecord_a1ba4& other) {
    fn_71000a1ba4(this, &other);
    return *this;
}

using UnkIdRecordPair = UnkValuePairRef<UnkStrIdTwoStep, UnkAssignedRecord_a1ba4>;

UnkIdRecordPair* fn_71000d6310(UnkIdRecordPair* self, UnkIdRecordPair* other) { return MoveAssignSet(self, other); }
UnkIdRecordPair* fn_71000d73b8(UnkIdRecordPair* self, UnkIdRecordPair* other) { return MoveAssignSet(self, other); }
