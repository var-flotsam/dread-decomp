// A family of track-value resolvers (one per value type, in several units; the first matched one is
// fn_7100b179f8 in src/game/GUI/IDisplayObjectTrack/IDisplayObjectTrack_ValueResolve_b46.cpp): a
// value of two parts, stored (kind 1) or given through a pointer to each part (kind 2), is moved or
// copied into the result, which is left empty (kind 0) for anything else. Names and layouts are
// placeholders.
#include "types.h"

template <class A, class B>
struct UnkResolveValue_b46a3 {
    A a;
    B b;
};

template <class A, class B>
struct UnkResolveRefs_b46a3 {
    A* a;
    B* b;
};

template <class A, class B>
struct UnkResolveVariant_b46a3 {
    union {
        UnkResolveValue_b46a3<A, B> value;
        UnkResolveRefs_b46a3<A, B> refs;
    };
    u32 kind;
};

// A trivially copyable value is copied whole in the stored case.
template <class A, class B>
static inline UnkResolveVariant_b46a3<A, B>* ResolveTrivial(UnkResolveVariant_b46a3<A, B>* out,
                                                            UnkResolveVariant_b46a3<A, B>* in) {
    out->kind = 0;
    switch (in->kind) {
    case 1:
        out->kind = 1;
        out->value = in->value;
        break;
    case 2: {
        out->kind = 1;
        UnkResolveRefs_b46a3<A, B> refs = in->refs;
        out->value.a = *refs.a;
        out->value.b = *refs.b;
        break;
    }
    }
    return out;
}

// Two floats, copied as one.
struct UnkResolvePair_b46a3 {
    f32 x, y;
};
using UnkResolveF32Pair_b46a3 = UnkResolveVariant_b46a3<f32, UnkResolvePair_b46a3>;

UnkResolveF32Pair_b46a3* fn_7100ffe02c(UnkResolveF32Pair_b46a3* out, UnkResolveF32Pair_b46a3* in) {
    return ResolveTrivial(out, in);
}
UnkResolveF32Pair_b46a3* fn_7100ffe2ac(UnkResolveF32Pair_b46a3* out, UnkResolveF32Pair_b46a3* in) {
    return ResolveTrivial(out, in);
}
