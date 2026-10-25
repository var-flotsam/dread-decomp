// Copies of fn_7100c8f544 (src/game_aca57c/fn_7100ad2728.cpp) for other record sizes, naming their own callees and data
// (the comments are adapted from the original's): fn_71000e0db4, fn_7100273ce4. A value and a pointer bound to a pair
// that refers to them (state 2) while the pointer is set. Each function builds one from the pointer held by its first
// argument and assigns it to the second: the value and the pointer are copied and the target's pair is rebound to the
// target's own members (or reset). The in-place second value is destroyed out of line. Two record sizes.
#include "base/global/CRntString.h"
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
void fn_710026ead0(void* self);
// The 0x30-byte values are strings (base/global/CRntString.h).
inline void DestroyString(void* self) { fn_710007ecd4(static_cast<base::global::CRntString*>(self)); }

using UnkBound30 = UnkDestroyed<0x30, DestroyString>;
using UnkBound50 = UnkDestroyed<0x50, fn_710026ead0>;

void fn_71000e0db4(UnkBound30* const* src, UnkBoundRef<UnkBound30>* out) { AssignBound(src, out); }
void fn_7100273ce4(UnkBound50* const* src, UnkBoundRef<UnkBound50>* out) { AssignBound(src, out); }
