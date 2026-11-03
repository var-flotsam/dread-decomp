// GUI::IDisplayObjectTrack unit (tools/tu_map.py unit 1671): four copies (one per track value
// type) of the function that resolves a track value: a stored value (kind 1) is copied, a value
// given through two pointers (kind 2) is read through them; the result is stored (kind 1) or left
// empty (kind 0). Names and layouts are placeholders.
#include "types.h"

struct UnkTrackPair_b46a3 {
    f32 x, y;
};

struct UnkTrackValue_b46a3 {
    f32 a;
    UnkTrackPair_b46a3 b;
};

struct UnkTrackRefs_b46a3 {
    const f32* a;
    const UnkTrackPair_b46a3* b;
};

struct UnkTrackSource_b46a3 {
    union {
        UnkTrackValue_b46a3 value;
        UnkTrackRefs_b46a3 refs;
    };
    u32 kind;
};

// The same storage as the source (a value or the two pointers), holding a value.
struct UnkTrackResult_b46a3 {
    union {
        UnkTrackValue_b46a3 value;
        UnkTrackRefs_b46a3 refs;
    };
    u32 kind;
};

template <int N>
struct UnkTrackResolver_b46a3 {
    static UnkTrackResult_b46a3* Resolve(UnkTrackResult_b46a3* out, const UnkTrackSource_b46a3* in) {
        out->kind = 0;
        switch (in->kind) {
        case 1:
            out->kind = 1;
            out->value = in->value;
            break;
        case 2: {
            out->kind = 1;
            UnkTrackRefs_b46a3 refs = in->refs;
            out->value.a = *refs.a;
            out->value.b = *refs.b;
            break;
        }
        }
        return out;
    }
};

UnkTrackResult_b46a3* fn_7100b179f8(UnkTrackResult_b46a3* out, const UnkTrackSource_b46a3* in) {
    return UnkTrackResolver_b46a3<0>::Resolve(out, in);
}
UnkTrackResult_b46a3* fn_7100b17c78(UnkTrackResult_b46a3* out, const UnkTrackSource_b46a3* in) {
    return UnkTrackResolver_b46a3<1>::Resolve(out, in);
}
UnkTrackResult_b46a3* fn_7100b191e8(UnkTrackResult_b46a3* out, const UnkTrackSource_b46a3* in) {
    return UnkTrackResolver_b46a3<2>::Resolve(out, in);
}
UnkTrackResult_b46a3* fn_7100b19450(UnkTrackResult_b46a3* out, const UnkTrackSource_b46a3* in) {
    return UnkTrackResolver_b46a3<3>::Resolve(out, in);
}
