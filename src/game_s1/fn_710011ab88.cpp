// Copies of fn_7100d3f5d4 (src/game_aca57c/fn_7100adae44.cpp) for another array type, naming their own callees and data
// (the comments are adapted from the original's): fn_710011ab88, fn_710011ac48, fn_710011b010, fn_710011b0d0. A value
// and an optional external array, with a pair referring to both: assignment copies the value and the array pointer and
// rebinds the pair. The in-place array (followed by 0x20 more bytes) frees its storage.
#include "base/UnkVector.h"
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

    // Destroys the in-place values (state 1); either way the pair is left empty.
    void Reset() {
        switch (state) {
        case 1:
            values.y.~Y();
            values.x.~X();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    // Refers to `x` and `y`.
    void Set(X* x, Y* y) {
        Reset();
        state = 2;
        refs.x = x;
        refs.y = y;
    }
};

template <class T>
struct UnkFreedVector : base::UnkVector<T> {
    ~UnkFreedVector() { this->Free(); }
};

struct UnkFreedVectorRecord {
    UnkFreedVector<u32> array;
    u64 unk_0x28[4];
};

struct UnkBoundValueArray {
    u32 value;
    UnkFreedVectorRecord* array;
    UnkValuePairRef<u32, UnkFreedVectorRecord> pair;
};
static_assert(__builtin_offsetof(UnkBoundValueArray, pair.state) == 0x60, "");

static inline UnkBoundValueArray* Assign(UnkBoundValueArray* self, const UnkBoundValueArray* other) {
    if (self != other) {
        self->value = other->value;
        self->array = other->array;
        if (self->array)
            self->pair.Set(&self->value, self->array);
        else
            self->pair.Reset();
    }
    return self;
}

UnkBoundValueArray* fn_710011ab88(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }
UnkBoundValueArray* fn_710011ac48(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }
UnkBoundValueArray* fn_710011b010(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }
UnkBoundValueArray* fn_710011b0d0(UnkBoundValueArray* self, const UnkBoundValueArray* other) { return Assign(self, other); }
