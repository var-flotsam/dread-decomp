// CCharClassLiquidPoolComponent unit: reflected iterator values (base/UnkCollectionIter.h) of two maps from a
// string id to a 4-float vector: their equality (class slot 0x58: fn_7100903a80, fn_71009040dc; the ids compared
// by pointer, the vectors within 1e-5 per component) and the first one's copy-assignment (slot 0x48,
// fn_7100903d34). Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace {

struct UnkLiquidKey_A4_b73 : base::global::CStrId {
    bool operator!=(const UnkLiquidKey_A4_b73& o) const { return data != o.data; }
};

inline bool NearlyEqual_A4_b73(f32 a, f32 b) {
    return a - 1e-5f <= b && a + 1e-5f >= b;
}

struct UnkLiquidVec4_A4_b73 {
    f32 x, y, z, w;
    UnkLiquidVec4_A4_b73& operator=(const UnkLiquidVec4_A4_b73& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        w = o.w;
        return *this;
    }
    bool operator==(const UnkLiquidVec4_A4_b73& o) const {
        return NearlyEqual_A4_b73(x, o.x) && NearlyEqual_A4_b73(y, o.y) && NearlyEqual_A4_b73(z, o.z) &&
               NearlyEqual_A4_b73(w, o.w);
    }
};

using UnkLiquidIterValue_A4_b73 = base::UnkIterValue<UnkLiquidVec4_A4_b73, UnkLiquidKey_A4_b73>;
static_assert(sizeof(UnkLiquidIterValue_A4_b73) == 0x20, "layout");

}  // namespace

bool fn_7100903a80(const void* a, const void* b) {
    return *static_cast<const UnkLiquidIterValue_A4_b73*>(a) == *static_cast<const UnkLiquidIterValue_A4_b73*>(b);
}

bool fn_71009040dc(const void* a, const void* b) {
    return *static_cast<const UnkLiquidIterValue_A4_b73*>(a) == *static_cast<const UnkLiquidIterValue_A4_b73*>(b);
}

void* fn_7100903d34(void* dst, const void* src) {
    return base::refl_IterValueCopy<UnkLiquidVec4_A4_b73, UnkLiquidKey_A4_b73>(dst, src);
}
