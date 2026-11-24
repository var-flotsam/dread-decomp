// CShockWaveComponent unit. fn_71009f4f48: the wave's spread direction, +x or -x by the sign of the
// linked object's transform angle y (+1 without one); while +0x189 is set and the angle at +0x168 is under
// a full turn, it starts from +y turned by the owner's angles instead (mirrored in x for +x). Then turned
// against an offset along z (+0x16c, opposite in sign for +x) and normalised in the xy plane.
// Names and layouts are placeholders.
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkShockWaveTarget_l9f4f48 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkShockWaveLink_l9f4f48 {
    u8 pad_0x0[0x10];
    UnkShockWaveTarget_l9f4f48* target;  // 0x10
};

struct UnkShockWave_l9f4f48 {
    u8 pad_0x0[0x30];
    UnkShockWaveTarget_l9f4f48* owner;  // 0x30
    u8 pad_0x38[0x168 - 0x38];
    f32 field_0x168;
    f32 field_0x16c;
    u8 pad_0x170[0x189 - 0x170];
    bool field_0x189;
    u8 pad_0x18a[0x1c8 - 0x18a];
    UnkShockWaveLink_l9f4f48* link;  // 0x1c8
};

inline UnkVec3f Turned(UnkVec3f* dir, const UnkVec3f& offset) {
    return fn_710007a450(dir, &offset);
}

UnkVec3f CShockWaveComponent_GetDirection(UnkShockWave_l9f4f48* self) {
    UnkVec3f dir(0.0f, 0.0f, 0.0f);
    bool positive = true;
    if (self->link) {
        if (UnkShockWaveTarget_l9f4f48* target = self->link->target)
            positive = target->xform.GetAngle().y > 0.0f;
    }
    if (positive) {
        dir = UnkVec3f(1.0f, 0.0f, 0.0f);
        if (self->field_0x189 && self->field_0x168 < 2.0 * 3.141592653589793) {
            dir = UnkVec3f(0.0f, 1.0f, 0.0f);
            UnkVec3f angle = self->owner->xform.GetAngle();
            UnkVec3f turned = Turned(&dir, angle);
            dir = UnkVec3f(-turned.x, turned.y, turned.z);
        }
        dir = Turned(&dir, UnkVec3f(0.0f, 0.0f, -self->field_0x16c));
    } else {
        dir = UnkVec3f(-1.0f, 0.0f, 0.0f);
        if (self->field_0x189 && self->field_0x168 < 2.0 * 3.141592653589793) {
            dir = UnkVec3f(0.0f, 1.0f, 0.0f);
            UnkVec3f angle = self->owner->xform.GetAngle();
            dir = Turned(&dir, angle);
        }
        dir = Turned(&dir, UnkVec3f(0.0f, 0.0f, self->field_0x16c));
    }
    UnkVec3f flat(dir.x, dir.y, 0.0f);
    dir = Normalized(flat);
    return dir;
}
