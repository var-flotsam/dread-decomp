// Two more members of the track-value resolver family (src/game/UnkTrackValueResolve_b46.cpp,
// fn_7100ffe02c), in unit 596 (tools/tu_map.py, no class hint): a word and a three-float vector,
// stored (kind 1) or given through a pointer to each part (kind 2), copied into the result, which is
// left empty (kind 0) for anything else. Names and layouts are placeholders.
#include "types.h"

struct UnkVec3_b64a2 {
    f32 x, y, z;
};

struct UnkResolveVec3Value_b64a2 {
    f32 a;
    UnkVec3_b64a2 b;
};

struct UnkResolveVec3Refs_b64a2 {
    f32* a;
    UnkVec3_b64a2* b;
};

struct UnkResolveVec3_b64a2 {
    union {
        UnkResolveVec3Value_b64a2 value;
        UnkResolveVec3Refs_b64a2 refs;
    };
    u32 kind;
};

static inline __attribute__((always_inline)) UnkResolveVec3_b64a2* ResolveVec3(UnkResolveVec3_b64a2* out,
                                                                                UnkResolveVec3_b64a2* in) {
    out->kind = 0;
    switch (in->kind) {
    case 1:
        out->kind = 1;
        out->value = in->value;
        break;
    case 2: {
        out->kind = 1;
        UnkResolveVec3Refs_b64a2 refs = in->refs;
        out->value.a = *refs.a;
        out->value.b = *refs.b;
        break;
    }
    }
    return out;
}

UnkResolveVec3_b64a2* fn_71003ed894(UnkResolveVec3_b64a2* out, UnkResolveVec3_b64a2* in) {
    return ResolveVec3(out, in);
}
UnkResolveVec3_b64a2* fn_71003edafc(UnkResolveVec3_b64a2* out, UnkResolveVec3_b64a2* in) {
    return ResolveVec3(out, in);
}
