// CSwarmControllerComponent's unit (tools/tu_map.py unit 1163): links a path point to the nearest other point
// seen so far. The squared distance between the two is kept per point; a nearer one replaces the old link.
// Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

// A path point: its path's index and its own index in the path.
struct UnkSwarmPair_ovb6 {
    u32 a;
    u32 b;
};

static inline bool SamePoint(const UnkSwarmPair_ovb6* x, const UnkSwarmPair_ovb6& y) {
    return x->b == y.b && x->a == y.a;
}

using UnkSwarmWordsAndPairs_ovb6 = base::global::CRntSmallDictionary<UnkSwarmPair_ovb6, u32>;
using UnkSwarmPairs2_ovb6 = base::global::CRntSmallDictionary<UnkSwarmPair_ovb6, UnkSwarmPair_ovb6>;
void fn_710074e938(UnkSwarmWordsAndPairs_ovb6* self, u32 n);
void fn_710074e82c(UnkSwarmPairs2_ovb6* self, u32 n);

// The squared distance of each point to the point it is linked to, kept as the float's bits.
using UnkSwarmDistances_749ad0 = UnkSwarmWordsAndPairs_ovb6;

// The value under key, or End().
template <class V>
static inline V* Find(base::global::CRntSmallDictionary<UnkSwarmPair_ovb6, V>* dict, const UnkSwarmPair_ovb6& key) {
    for (u32 i = 0; i < dict->count; ++i)
        if (SamePoint(&dict->keys[i], key))
            return &dict->values[i];
    return dict->End();
}

// Removes the entry under key, if there is one, moving the last entry into its place.
template <class V>
static inline void Remove(base::global::CRntSmallDictionary<UnkSwarmPair_ovb6, V>* dict, const UnkSwarmPair_ovb6& key) {
    V* it = Find(dict, key);
    if (it == dict->End())
        return;
    u32 index = it - dict->values;
    if (index != dict->count - 1) {
        dict->values[index] = dict->values[dict->count - 1];
        dict->keys[index] = dict->keys[dict->count - 1];
    }
    --dict->count;
}

static inline void Add(UnkSwarmPairs2_ovb6* dict, const UnkSwarmPair_ovb6& key, const UnkSwarmPair_ovb6& value) {
    if (dict->count == dict->capacity)
        fn_710074e82c(dict, dict->step + dict->count);
    dict->keys[dict->count] = key;
    dict->values[dict->count] = value;
    ++dict->count;
}

static inline void Add(UnkSwarmDistances_749ad0* dict, const UnkSwarmPair_ovb6& key, f32 value) {
    if (dict->count == dict->capacity)
        fn_710074e938(dict, dict->step + dict->count);
    dict->keys[dict->count] = key;
    dict->values[dict->count] = *reinterpret_cast<const u32*>(&value);
    ++dict->count;
}

struct UnkSwarmPoint_749ad0 {
    void** vtable;
    const CVector3D* Position() {
        return reinterpret_cast<const CVector3D* (*)(UnkSwarmPoint_749ad0*)>(vtable[0x18 / 8])(this);
    }
};

struct UnkSwarmPath_749ad0 {
    void** vtable;
    UnkSwarmPoint_749ad0* Point(u32 index) {
        return reinterpret_cast<UnkSwarmPoint_749ad0* (*)(UnkSwarmPath_749ad0*, u32)>(vtable[0x30 / 8])(this, index);
    }
};

struct UnkSwarmPaths_749ad0 {
    void** vtable;
    UnkSwarmPath_749ad0* Path(u32 index) {
        return reinterpret_cast<UnkSwarmPath_749ad0* (*)(UnkSwarmPaths_749ad0*, u32)>(vtable[0x30 / 8])(this, index);
    }
    const CVector3D* Position(const UnkSwarmPair_ovb6& point) { return Path(point.a)->Point(point.b)->Position(); }
};

struct UnkSwarmController_749ad0 {
    u8 pad_0x0[0x80];
    UnkSwarmPaths_749ad0* paths;  // 0x80
    u8 pad_0x88[0xf0 - 0x88];
    UnkSwarmPairs2_ovb6 links;  // 0xf0: each point's linked point
};

void fn_7100749ad0(UnkSwarmController_749ad0* self, UnkSwarmDistances_749ad0* distances,
                   const UnkSwarmPair_ovb6* point, const UnkSwarmPair_ovb6* other) {
    u32* known = Find(distances, *point);
    const CVector3D* to = self->paths->Position(*other);
    const CVector3D* from = self->paths->Position(*point);
    f32 dx = to->x - from->x;
    f32 dy = to->y - from->y;
    f32 dz = to->z - from->z;
    f32 distance_sq = dx * dx + dy * dy + dz * dz;
    if (known != distances->End()) {
        if (distance_sq > *reinterpret_cast<const f32*>(known))
            return;
        Remove(&self->links, *point);
        Remove(distances, *point);
    }
    Add(&self->links, *point, *other);
    Add(distances, *point, distance_sq);
}
