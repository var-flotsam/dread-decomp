// CScourgeAIComponent's unit (tools/tu_map.py): passes the owner's (+0x30) position and two axes of
// its transform matrix (fn_710002614c; the third and the second column) on to fn_710069f728 with
// the two arguments. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;

struct UnkScourgeAI_b66a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

bool fn_710069f728(UnkScourgeAI_b66a4* self, const CVector3D* pos, const CVector3D* axisA, const CVector3D* axisB,
                   void* a, void* b);

static inline __attribute__((always_inline)) CVector3D Position_b66a4(const UnkActor* a) {
    CVector3D v;
    v.x = a->field_0x44;
    v.y = a->field_0x54;
    v.z = a->field_0x64;
    return v;
}

static inline __attribute__((always_inline)) CVector3D Column_b66a4(const UnkMatrix44& m, int i) {
    CVector3D v;
    v.x = m.m[i];
    v.y = m.m[4 + i];
    v.z = m.m[8 + i];
    return v;
}

static inline __attribute__((always_inline)) const UnkMatrix44& Matrix_b66a4(UnkActor* a) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(a->field_0x38));
}

bool fn_710069f680(UnkScourgeAI_b66a4* self, void* a, void* b) {
    CVector3D pos = Position_b66a4(self->owner);
    CVector3D axisA = Column_b66a4(Matrix_b66a4(self->owner), 2);
    CVector3D axisB = Column_b66a4(Matrix_b66a4(self->owner), 1);
    return fn_710069f728(self, &pos, &axisA, &axisB, a, b);
}
