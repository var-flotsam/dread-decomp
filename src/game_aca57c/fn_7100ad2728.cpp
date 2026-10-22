// A value and a pointer bound to a pair that refers to them (state 2) while the pointer is set.
// Each function builds one from the pointer held by its first argument and assigns it to the
// second: the value and the pointer are copied and the target's pair is rebound to the target's
// own members (or reset). The in-place second value is destroyed out of line. Three record
// sizes.
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

template <u32 Size, void (*Destroy)(void*)>
struct UnkDestroyed {
    u64 unk_0x0[Size / 8];

    __attribute__((always_inline)) ~UnkDestroyed() { Destroy(this); }
};

void fn_7100ad04c4(void* self);
void fn_7100c886c0(void* self);
void fn_7100c8a490(void* self);

using UnkBound80 = UnkDestroyed<0x80, fn_7100ad04c4>;
using UnkBoundA0 = UnkDestroyed<0xa0, fn_7100c886c0>;
using UnkBound40 = UnkDestroyed<0x40, fn_7100c8a490>;

void fn_7100ad2728(UnkBound80* const* src, UnkBoundRef<UnkBound80>* out) { AssignBound(src, out); }
void fn_7100c8f544(UnkBoundA0* const* src, UnkBoundRef<UnkBoundA0>* out) { AssignBound(src, out); }
void fn_7100c92950(UnkBound40* const* src, UnkBoundRef<UnkBound40>* out) { AssignBound(src, out); }
