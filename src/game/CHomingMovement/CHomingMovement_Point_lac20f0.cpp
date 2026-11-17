// CHomingMovement unit (tools/tu_map.py unit 1622): the point to home in on. With a point name
// (+0x5e0), the target's model (+0x228) answers where that point is (fn_710117364c, zero when it does
// not); without one, it is the target's position. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkHomingMovement_lac20f0 {
    u8 pad_0x0[0x5d8];
    UnkWeakHandle* target;  // 0x5d8
    CStrId point;           // 0x5e0
};

// Asks a model where its point `id` is.
inline bool FindPoint(void* model, const u64& id, UnkVec3f* out) {
    return fn_710117364c(model, &id, reinterpret_cast<UnkGrappleAnswer_b50*>(out));
}

static inline UnkActor* Target(UnkHomingMovement_lac20f0* self) {
    return reinterpret_cast<UnkActor*>(self->target->target);
}

UnkVec3f fn_7100ac20f0(UnkHomingMovement_lac20f0* self) {
    UnkVec3f point(0.0f, 0.0f, 0.0f);
    if (self->point.data != &g_unk_7101d47108->empty) {
        FindPoint(Target(self)->field_0x228, self->point.data->Key(), &point);
    } else {
        UnkActor* target = Target(self);
        point = UnkVec3f(target->field_0x44, target->field_0x54, target->field_0x64);
    }
    return point;
}
