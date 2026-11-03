// Getters returning a copy of a constant three-word vector (unit 132 of tools/tu_map.py): two
// return the zero vector g_unk_710164779c, one the vector after it. The result
// is a class with a user-written copy constructor (returned through x8, copied word by word). Names are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "types.h"

extern const base::math::CVector3D g_unk_71016477a8;

struct UnkVector3Words_b42a1 {
    u32 x, y, z;

    UnkVector3Words_b42a1(const UnkVector3Words_b42a1& other) : x(other.x), y(other.y), z(other.z) {}
};

static inline const UnkVector3Words_b42a1& Words(const base::math::CVector3D& v) {
    return reinterpret_cast<const UnkVector3Words_b42a1&>(v);
}

UnkVector3Words_b42a1 fn_7100145864() { return Words(g_unk_710164779c); }
UnkVector3Words_b42a1 fn_7100145880() { return Words(g_unk_710164779c); }
UnkVector3Words_b42a1 fn_710014589c() { return Words(g_unk_71016477a8); }
