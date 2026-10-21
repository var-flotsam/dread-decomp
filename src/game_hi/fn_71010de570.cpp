// Sorts five entries in place (libc++ __sort5 shape: sort the first four, then insert the fifth)
// and returns the number of swaps. Same ordering as fn_71010dde80.
#include "types.h"

struct UnkClass;

struct UnkReflObj {
    virtual UnkClass* vf_0x0();
};

struct UnkInner_0x18 {
    u8 pad_0x0[0x10];
    UnkReflObj* field_0x10;
};

struct UnkOwner_0x18 {
    u8 pad_0x0[0x10];
    UnkInner_0x18* field_0x10;
};

struct UnkEntry_0x3c {
    UnkOwner_0x18* field_0x0;
    u8 pad_0x8[0x30];
    f32 field_0x38;
};

namespace CBreakableTileGroup {
UnkClass* refl_GetClass();
}
bool fn_710009d50c(UnkClass*, UnkClass*);

static inline bool IsTileGroup(const UnkEntry_0x3c* e) {
    if (UnkOwner_0x18* o = e->field_0x0)
        if (UnkInner_0x18* i = o->field_0x10)
            if (UnkReflObj* r = i->field_0x10)
                return fn_710009d50c(r->vf_0x0(), CBreakableTileGroup::refl_GetClass());
    return false;
}

static inline bool Near(const f32& x, const f32& y) { return x - 1e-5f <= y && x + 1e-5f >= y; }
static inline bool Less(const UnkEntry_0x3c* a, const UnkEntry_0x3c* b) {
    if (Near(a->field_0x38, b->field_0x38))
        return !IsTileGroup(a) & IsTileGroup(b);
    return false;
}

template <class T>
static inline void Swap(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

u32 fn_71010de270(UnkEntry_0x3c** x1, UnkEntry_0x3c** x2, UnkEntry_0x3c** x3, UnkEntry_0x3c** x4);

u32 fn_71010de570(UnkEntry_0x3c** x1, UnkEntry_0x3c** x2, UnkEntry_0x3c** x3, UnkEntry_0x3c** x4,
                  UnkEntry_0x3c** x5) {
    u32 r = fn_71010de270(x1, x2, x3, x4);
    if (Less(*x5, *x4)) {
        Swap(*x4, *x5);
        ++r;
        if (Less(*x4, *x3)) {
            Swap(*x3, *x4);
            ++r;
            if (Less(*x3, *x2)) {
                Swap(*x2, *x3);
                ++r;
                if (Less(*x2, *x1)) {
                    Swap(*x1, *x2);
                    ++r;
                }
            }
        }
    }
    return r;
}
