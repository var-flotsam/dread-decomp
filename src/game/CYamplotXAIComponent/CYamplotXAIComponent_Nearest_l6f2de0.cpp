// CYamplotXAIComponent unit: which of three candidate points is nearest, in the plane, to the owner's
// resolved actor; falls back to the base class's choice when there is no actor.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkYamplotXCandidate_l6f2de0 {
    s16 id;    // 0x0, -1 for none
    u8 slot;   // 0x2, 0xff for none
    u8 pad_0x3[9];
    f32 x;     // 0xc
    f32 y;     // 0x10
};

struct UnkYamplotXPosOverride_l6f2de0;

struct UnkYamplotXAIView_l6f2de0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

int fn_710110a6bc(UnkYamplotXAIView_l6f2de0* self, const UnkYamplotXCandidate_l6f2de0* candidates,
                  const UnkYamplotXPosOverride_l6f2de0* over);

inline f32 DistanceSq(const UnkYamplotXCandidate_l6f2de0* c, const CVector2D& pos) {
    if (c->id == -1 || c->slot == 0xff)
        return 3.4028235e38f;
    f32 dx = pos.x - c->x;
    f32 dy = pos.y - c->y;
    return dx * dx + dy * dy;
}

inline CVector3D Position(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

int fn_71006f2de0(UnkYamplotXAIView_l6f2de0* self, const UnkYamplotXCandidate_l6f2de0* candidates,
                  const UnkYamplotXPosOverride_l6f2de0* over) {
    int nearest = fn_710110a6bc(self, candidates, over);
    UnkActor* actor = fn_7100d7c990(self->owner);
    if (actor == nullptr)
        return nearest;

    CVector3D position = Position(actor);
    CVector2D pos;
    fn_710007b430(&pos, &position);

    f32 d0 = DistanceSq(&candidates[0], pos);
    f32 d1 = DistanceSq(&candidates[1], pos);
    f32 d2 = DistanceSq(&candidates[2], pos);
    if (d0 <= d1 && d0 <= d2)
        return 0;
    if (d1 <= d2 && d1 <= d0)
        return 1;
    return 2;
}
