// CSamusMagnetGloveMovementState::CParams's unit (tools/tu_map.py unit 1466): the state's update. The
// tracked object's move since the last update (+0x1f8), flattened to two components (fn_710007b430), is added
// to the two offsets at +0x1d0 and +0x1d8; after the step (fn_71009ab50c) the position is kept for next time,
// and when the owner's animation entry for one key changed, fn_710083daa4 runs on the owner's +0x2a8.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

void fn_710083daa4(void* self);

struct UnkMagnetGloveOwner_l9ab3e8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x80];
    void* field_0x2a8;  // 0x2a8
};

struct UnkMagnetGloveTracked_l9ab3e8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkMagnetGloveTarget_l9ab3e8 {
    u8 pad_0x0[0x10];
    UnkMagnetGloveTracked_l9ab3e8* tracked;  // 0x10
};

struct UnkMagnetGloveState_l9ab3e8 {
    u8 pad_0x0[0x30];
    UnkMagnetGloveOwner_l9ab3e8* owner;  // 0x30
    u8 pad_0x38[0x198];
    CVector2D offset;      // 0x1d0
    CVector2D offset2;     // 0x1d8
    u8 pad_0x1e0[0x8];
    UnkMagnetGloveTarget_l9ab3e8* target;  // 0x1e8
    u8 pad_0x1f0[0x8];
    UnkVec3f last;  // 0x1f8
};
static_assert(__builtin_offsetof(UnkMagnetGloveState_l9ab3e8, last) == 0x1f8, "layout");

void fn_71009ab50c(UnkMagnetGloveState_l9ab3e8* self, void* arg);

static const u64 kAnimationKey = 0xf4061ff25a2163d6;

void fn_71009ab3e8(UnkMagnetGloveState_l9ab3e8* self, void* arg) {
    void* before = fn_7101177d2c(self->owner->animation, kAnimationKey);
    UnkMagnetGloveTracked_l9ab3e8* tracked = self->target->tracked;
    UnkVec3f pos(tracked->x, tracked->y, tracked->z);
    CVector3D delta = {tracked->x - self->last.x, tracked->y - self->last.y, tracked->z - self->last.z};
    CVector2D move;
    fn_710007b430(&move, &delta);
    self->offset.x = move.x + self->offset.x;
    self->offset2.x = move.x + self->offset2.x;
    self->offset.y = move.y + self->offset.y;
    self->offset2.y = move.y + self->offset2.y;
    fn_71009ab50c(self, arg);
    self->last = pos;
    if (before != fn_7101177d2c(self->owner->animation, kAnimationKey))
        fn_710083daa4(self->owner->field_0x2a8);
}
