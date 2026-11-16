// Unit 54 (tools/tu_map.py): blends two transforms by `weight`: each is split into translation,
// rotation (a quaternion) and scale, translation and scale are interpolated linearly and the
// rotations normalized-linearly, and the parts are put back together. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVectorOps.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// A 3-component vector in a 16-byte slot; the fourth word is never set.
struct UnkPaddedVec3 {
    f32 x, y, z;
    u32 pad_0xc;

    UnkPaddedVec3() : x(0.0f), y(0.0f), z(0.0f) {}
};

// Splits a transform matrix into translation, rotation and scale.
void fn_7100077620(const UnkMatrix44* m, UnkPaddedVec3* translation,
                   base::math::CVector4D* rotation, UnkPaddedVec3* scale);

inline UnkVec3f LerpVec3(const UnkPaddedVec3& a, const UnkPaddedVec3& b, f32 t) {
    if (t < 1.0f)
        return UnkVec3f((1.0f - t) * a.x + b.x * t, (1.0f - t) * a.y + b.y * t,
                        (1.0f - t) * a.z + b.z * t);
    return UnkVec3f(b.x, b.y, b.z);
}


inline UnkMatrix44 TranslationMatrix(const UnkVec3f& t) {
    UnkMatrix44 m;
    m.m[0] = 1.0f;
    m.m[1] = 0.0f;
    m.m[2] = 0.0f;
    m.m[3] = t.x;
    m.m[4] = 0.0f;
    m.m[5] = 1.0f;
    m.m[6] = 0.0f;
    m.m[7] = t.y;
    m.m[8] = 0.0f;
    m.m[9] = 0.0f;
    m.m[10] = 1.0f;
    m.m[11] = t.z;
    m.m[12] = 0.0f;
    m.m[13] = 0.0f;
    m.m[14] = 0.0f;
    m.m[15] = 1.0f;
    return m;
}

inline UnkMatrix44 RotationMatrix(const base::math::CVector4D& q) {
    UnkMatrix44 m;
    m.m[0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    m.m[1] = 2.0f * (q.x * q.y - q.z * q.w);
    m.m[2] = 2.0f * (q.x * q.z + q.y * q.w);
    m.m[3] = 0.0f;
    m.m[4] = 2.0f * (q.x * q.y + q.z * q.w);
    m.m[5] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    m.m[6] = 2.0f * (q.y * q.z - q.x * q.w);
    m.m[7] = 0.0f;
    m.m[8] = 2.0f * (q.x * q.z - q.y * q.w);
    m.m[9] = 2.0f * (q.y * q.z + q.x * q.w);
    m.m[10] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    m.m[11] = 0.0f;
    m.m[12] = 0.0f;
    m.m[13] = 0.0f;
    m.m[14] = 0.0f;
    m.m[15] = 1.0f;
    return m;
}

inline UnkMatrix44 ScaleMatrix(const UnkVec3f& s) {
    UnkMatrix44 m;
    m.m[0] = s.x;
    m.m[1] = 0.0f;
    m.m[2] = 0.0f;
    m.m[3] = 0.0f;
    m.m[4] = 0.0f;
    m.m[5] = s.y;
    m.m[6] = 0.0f;
    m.m[7] = 0.0f;
    m.m[8] = 0.0f;
    m.m[9] = 0.0f;
    m.m[10] = s.z;
    m.m[11] = 0.0f;
    m.m[12] = 0.0f;
    m.m[13] = 0.0f;
    m.m[14] = 0.0f;
    m.m[15] = 1.0f;
    return m;
}

UnkMatrix44 fn_7100078834(const UnkKeyedEntry* entry, const UnkMatrix44* matrix, f32 weight) {
    UnkPaddedVec3 fromTranslation;
    UnkPaddedVec3 toTranslation;
    base::math::UnkVector4D fromRotation;
    base::math::UnkVector4D toRotation;
    base::math::UnkVector4D rotation;
    UnkPaddedVec3 fromScale;
    UnkPaddedVec3 toScale;
    fn_7100077620((const UnkMatrix44*)entry, &fromTranslation, &fromRotation, &fromScale);
    fn_7100077620(matrix, &toTranslation, &toRotation, &toScale);

    UnkVec3f translation = LerpVec3(fromTranslation, toTranslation, weight);

    // Take the shorter way round: flip the target rotation when the two point apart.
    f32 toWeight = Dot(fromRotation, toRotation) >= 0.0f
                       ? weight
                       : -weight;
    {
        base::math::UnkVector4D blended = fromRotation * (1.0f - weight) + toRotation * toWeight;
        Assign(rotation, Normalized(blended));
    }

    UnkVec3f scale = LerpVec3(fromScale, toScale, weight);

    UnkMatrix44 transform = fn_71000127c8(TranslationMatrix(translation), RotationMatrix(rotation));
    return fn_71000127c8(transform, ScaleMatrix(scale));
}
