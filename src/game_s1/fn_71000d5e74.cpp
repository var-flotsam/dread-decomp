// A copy of fn_7100ad2728 (src/game_aca57c/fn_7100ad2728.cpp) for 0x18-byte records destroyed by
// fn_71000a1c60: a value and a pointer bound to a pair that refers to them (state 2) while the
// pointer is set, built from the pointer its first argument holds and assigned to the second:
// fn_71000d5e74.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

template <class X, class Y>
struct UnkBoundPair {
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

    __attribute__((always_inline)) UnkBoundPair() {}
    __attribute__((always_inline)) ~UnkBoundPair() {}

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            values.y.~Y();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
    __attribute__((always_inline)) void Set(X* x, Y* y) {
        Reset();
        state = 2;
        refs.x = x;
        refs.y = y;
    }
};

template <class Y>
struct UnkBoundRef {
    u32 value;
    Y* ptr;
    UnkBoundPair<u32, Y> pair;

    __attribute__((always_inline)) explicit UnkBoundRef(Y* p) : value(0), ptr(p) {
        pair.state = 0;
        if (ptr)
            pair.Set(&value, ptr);
    }
    __attribute__((always_inline)) UnkBoundRef& operator=(const UnkBoundRef& other) {
        if (this != &other) {
            value = other.value;
            ptr = other.ptr;
            if (ptr)
                pair.Set(&value, ptr);
            else
                pair.Reset();
        }
        return *this;
    }
    __attribute__((always_inline)) ~UnkBoundRef() { pair.Reset(); }
};

template <class Y>
__attribute__((always_inline)) static inline void AssignBound(Y* const* src, UnkBoundRef<Y>* out) {
    *out = UnkBoundRef<Y>(*src);
}

template <u32 Size, class T, void (*Destroy)(T*)>
struct UnkDestroyed {
    u64 unk_0x0[Size / 8];

    __attribute__((always_inline)) ~UnkDestroyed() { Destroy(static_cast<T*>(static_cast<void*>(this))); }
};

using UnkBound18 = UnkDestroyed<0x18, base::reflection::UnkReflValue, fn_71000a1c60>;

void fn_71000d5e74(UnkBound18* const* src, UnkBoundRef<UnkBound18>* out) { AssignBound(src, out); }

// The same for 0x88-byte values destroyed by fn_71007a8eec, in another unit.
void fn_71007a8eec(void* self);
using UnkBound88 = UnkDestroyed<0x88, void, fn_71007a8eec>;

void fn_71007ace64(UnkBound88* const* src, UnkBoundRef<UnkBound88>* out) { AssignBound(src, out); }
