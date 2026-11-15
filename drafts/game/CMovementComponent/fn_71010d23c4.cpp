// CMovementComponent unit: turns a displacement (b) into the one the owner's floor track allows, in a, when the
// component is not paused: the result starts as (0, 0, b.z); the track's first flag adds b.x of the direction a,
// else b.x on x; its second flag adds b.y of the direction rotated a quarter turn about up, else b.y on y.
// The result is b itself when the component is paused or has no track. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

u32 fn_7101179808(char* p0);

struct UnkDisplaceRecord_l0d23c4 {
    u8 pad_0x0[0x18];
    u8 use_direction;  // 0x18
    u8 use_side;       // 0x19
};

struct UnkDisplaceOwner_l0d23c4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* anim;  // 0x220
};

struct UnkDisplaceLink_l0d23c4 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkDisplace_l0d23c4 {
    u8 pad_0x0[0x30];
    UnkDisplaceOwner_l0d23c4* owner;  // 0x30
    u8 pad_0x38[0xc1];
    u8 paused;  // 0xf9
    u8 pad_0xfa[0x2e];
    UnkDisplaceLink_l0d23c4* link;  // 0x128
};

UnkVec3f fn_71010d23c4(UnkDisplace_l0d23c4* self, UnkVec3f* a, UnkVec3f* b) {
    UnkVec3f result(*b);
    if (self->paused)
        return result;
    if (!self->link || !self->link->field_0x10)
        return result;
    if (!self->owner->anim)
        return result;
    if (!(fn_7101179808(reinterpret_cast<char*>(self->owner->anim)) & 1))
        return result;
    UnkTrackQuery query = {fn_7100d24d58(), nullptr, -1.0f};
    if (!self->owner->anim)
        return result;
    fn_710117b08c(self->owner->anim, &query);
    UnkDisplaceRecord_l0d23c4* rec = reinterpret_cast<UnkDisplaceRecord_l0d23c4*>(query.result);
    if (!rec)
        return result;
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = b->z;
    f32 rx = result.x, ry = result.y, rz = result.z;
    if (rec->use_direction) {
        rx = b->x * a->x + 0.0f;
        ry = b->x * a->y + 0.0f;
        rz = b->x * a->z + rz;
    } else {
        rx = b->x + 0.0f;
    }
    if (rec->use_side) {
        f32 z0 = a->z * 0.0f;
        f32 cx = z0 - a->y;
        f32 cy = a->x - z0;
        f32 cz = a->y * 0.0f - a->x * 0.0f;
        rx = cx * b->y + rx;
        ry = cy * b->y + ry;
        rz = b->y * cz + rz;
    } else {
        ry = b->y + ry;
    }
    result.x = rx;
    result.y = ry;
    result.z = rz;
    return result;
}
