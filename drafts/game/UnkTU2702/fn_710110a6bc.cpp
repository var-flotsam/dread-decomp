// Unit 2702 (CAIComponent): which of three candidate points is nearest to the owner's position (or
// to a given one). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkOwnerPose_l10a6bc {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkOwner_l10a6bc {
    u8 pad_0x0[0x30];
    UnkOwnerPose_l10a6bc* pose;  // 0x30
};

struct UnkCandidate_l10a6bc {
    s16 id;  // 0x0, -1 for none
    u8 slot;  // 0x2, 0xff for none
    u8 pad_0x3[9];
    f32 x;  // 0xc
    f32 y;  // 0x10
};

struct UnkPosOverride_l10a6bc {
    CVector2D pos;
    bool valid;  // 0x8
};

void fn_710007b430(CVector2D* out, const CVector3D* v);

static inline f32 DistanceSq(const UnkCandidate_l10a6bc* c, const CVector2D& pos) {
    if (c->id == -1 || c->slot == 0xff)
        return 3.4028235e38f;
    f32 dx = pos.x - c->x;
    f32 dy = pos.y - c->y;
    return dx * dx + dy * dy;
}

int fn_710110a6bc(UnkOwner_l10a6bc* self, const UnkCandidate_l10a6bc* candidates, const UnkPosOverride_l10a6bc* over) {
    CVector2D pos;
    f32 x = self->pose->x;
    f32 y = self->pose->y;
    f32 z = self->pose->z;
    CVector3D v = {x, y, z};
    fn_710007b430(&pos, &v);
    if (over && over->valid) {
        pos.x = over->pos.x;
        pos.y = over->pos.y;
    }
    f32 d0 = DistanceSq(&candidates[0], pos);
    f32 d1 = DistanceSq(&candidates[1], pos);
    f32 d2 = DistanceSq(&candidates[2], pos);
    if (!(d0 > d1) && !(d0 > d2))
        return 0;
    return (!(d1 > d2) && !(d1 > d0)) ? 1 : 2;
}
