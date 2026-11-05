// Unit 2574: three getters returning a copy of the constant vector g_unk_71016477a8 (as
// src/base/UnkUnit132/UnkVector3Getters_b42.cpp: a class with a user-written copy constructor, returned
// through x8). Names are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "types.h"

extern const base::math::CVector3D g_unk_71016477a8;

struct UnkVector3Words_b65a6 {
    u32 x, y, z;

    UnkVector3Words_b65a6(const UnkVector3Words_b65a6& other) : x(other.x), y(other.y), z(other.z) {}
};

static inline const UnkVector3Words_b65a6& Words_b65a6(const base::math::CVector3D& v) {
    return reinterpret_cast<const UnkVector3Words_b65a6&>(v);
}

UnkVector3Words_b65a6 fn_7100ecf7a0() { return Words_b65a6(g_unk_71016477a8); }
UnkVector3Words_b65a6 fn_7100ecf858() { return Words_b65a6(g_unk_71016477a8); }
UnkVector3Words_b65a6 fn_7100ecfaa0() { return Words_b65a6(g_unk_71016477a8); }
