// CKraidAcidBlobsMovementComponent's unit (tools/tu_map.py): the blob's update. After the base update
// (fn_7100ac3a78), a blob still in flight spins its owner about z, faster for a larger size step (+0x5e8,
// kept in 0..13) and the way its velocity points; then the trail part is placed as at launch. Names and
// layouts are placeholders.
#include "game/CFXComponent.h"
#include "game/CProjectileMovement.h"
#include "game/UnkAcidBlobDef_ovb0135.h"
#include "game/UnkAcidBlobOwner_ovb0135.h"
#include "game/UnkAcidBlobPart_ovb0135.h"
#include "game/UnkAcidBlobTarget_ovb0135.h"
#include "game/UnkAcidBlob_ovb0135.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU133.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkObject;
struct UnkPart;
extern "C" f32 atan2f(f32 y, f32 x);

static_assert(__builtin_offsetof(UnkAcidBlobOwner_ovb0135, dirty) == 0x3c0, "layout");

static_assert(__builtin_offsetof(UnkAcidBlob_ovb0135, def) == 0x5d0, "layout");

// Places the owner's trail part (fn_7101152b90) at the owner, turned along the direction at +0x10c.
static inline void PlaceTrail(UnkAcidBlob_ovb0135* self) {
    UnkObject* model = self->owner->model;
    auto* part = reinterpret_cast<UnkAcidBlobPart_ovb0135*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(model),
                                                                          0x1ce2163e2d5b53c6ull));
    if (!part || !fn_710114e680(model, part->kind))
        return;
    UnkVec3f angles;
    angles.x = 0.0f;
    angles.y = 1.57f;
    f32 dx = self->dir_x, dy = self->dir_y, dz = self->dir_z;
    f32 inv = 1.0f / (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    f32 nx = dx * inv;
    f32 ny = dy * inv;
    angles.z = atan2f(ny, nx);
    UnkAcidBlobOwner_ovb0135* owner = self->owner;
    UnkVec3f at(owner->xform.m[0][3], owner->xform.m[1][3], owner->xform.m[2][3]);
    UnkVec3f pos = at;
    fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(part), &pos);
    fn_7100145f3c(reinterpret_cast<UnkSwitchObject*>(part), &angles);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71006049a4(UnkAcidBlob_ovb0135* self, f32 dt) {
    fn_7100ac3a78(reinterpret_cast<UnkNailongThornMovement*>(self), dt);
    if (!self->landed) {
        UnkVec3f angle = self->owner->xform.GetAngle();
        s32 step = self->size_step < 13 ? self->size_step : 13;
        step = step > 0 ? step : 0;
        self->size_step = step;
        f32 rate = static_cast<f32>(step) / 13.0f * 0.5f + 0.5f;
        f32 turn = self->def->spin * (dt * 6.2831855f * rate);
        angle.z = (self->vel_x < 0.0f ? turn : -turn) + angle.z;
        UnkAcidBlobOwner_ovb0135* owner = self->owner;
        owner->xform.SetAngle(angle);
        owner->dirty = true;
    }
    PlaceTrail(self);
}
