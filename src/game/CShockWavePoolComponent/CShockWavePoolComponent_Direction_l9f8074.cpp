// CShockWavePoolComponent unit. fn_71009f8074: the pool's spread direction, +x or -x by the sign of the
// linked object's transform angle y (+1 without one), turned by fn_710007a450 against an offset along z
// (+0x164, opposite in sign) and normalised in the xy plane. Names and layouts are placeholders.
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkShockWavePoolTarget_l9f8074 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkShockWavePoolLink_l9f8074 {
    u8 pad_0x0[0x10];
    UnkShockWavePoolTarget_l9f8074* target;  // 0x10
};

struct UnkShockWavePool_l9f8074 {
    u8 pad_0x0[0x164];
    f32 field_0x164;
    u8 pad_0x168[0x1c8 - 0x168];
    UnkShockWavePoolLink_l9f8074* link;  // 0x1c8
};

inline UnkVec3f Turned(UnkVec3f* dir, const UnkVec3f& offset) {
    return fn_710007a450(dir, &offset);
}

UnkVec3f CShockWavePoolComponent_GetDirection(UnkShockWavePool_l9f8074* self) {
    UnkVec3f dir(0.0f, 0.0f, 0.0f);
    bool positive = true;
    if (self->link) {
        if (UnkShockWavePoolTarget_l9f8074* target = self->link->target)
            positive = target->xform.GetAngle().y > 0.0f;
    }
    if (positive) {
        dir = UnkVec3f(1.0f, 0.0f, 0.0f);
        dir = Turned(&dir, UnkVec3f(0.0f, 0.0f, -self->field_0x164));
    } else {
        dir = UnkVec3f(-1.0f, 0.0f, 0.0f);
        dir = Turned(&dir, UnkVec3f(0.0f, 0.0f, self->field_0x164));
    }
    UnkVec3f flat(dir.x, dir.y, 0.0f);
    dir = Normalized(flat);
    return dir;
}
