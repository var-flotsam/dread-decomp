// Sorts three entries in place (libc++ __sort3 shape) and returns the number of swaps. The order
// puts an entry before another at (nearly) the same depth when only the second one belongs to a
// breakable tile group.
#include "base/reflection/UnkReflCast.h"
#include "game/UnkEntry_0x3c.h"
#include "game/UnkInner_0x18.h"
#include "game/UnkOwner_0x18.h"
#include "types.h"

using base::reflection::CClass;

struct UnkReflObj {
    virtual const CClass* vf_0x0();
};

namespace CBreakableTileGroup {
const CClass* refl_GetClass();
}

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

u32 fn_71010dde80(UnkEntry_0x3c** x, UnkEntry_0x3c** y, UnkEntry_0x3c** z) {
    u32 r = 0;
    if (!Less(*y, *x)) {
        if (!Less(*z, *y))
            return r;
        Swap(*y, *z);
        r = 1;
        if (Less(*y, *x)) {
            Swap(*x, *y);
            r = 2;
        }
        return r;
    }
    if (Less(*z, *y)) {
        Swap(*x, *z);
        r = 1;
        return r;
    }
    Swap(*x, *y);
    r = 1;
    if (Less(*z, *y)) {
        Swap(*y, *z);
        r = 2;
    }
    return r;
}
