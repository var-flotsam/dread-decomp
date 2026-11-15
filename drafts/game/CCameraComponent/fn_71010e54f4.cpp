// CCameraComponent (unit 2691): the 2D box the view covers around the first object's position,
// from the field of view; false when there is none. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU311.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkCamViewBox_l0e54f4 {
    f32 minX, minY, maxX, maxY;
};

struct UnkCamTarget_l0e54f4 {
    u8 pad_0x0[0x68];
    UnkVec3f position;
};

struct UnkCamNode_l0e54f4 {
    UnkCamTarget_l0e54f4* value;
};

struct UnkCamComponent_l0e54f4 {
    u8 pad_0x0[0x60];
    UnkCamNode_l0e54f4* nodes;
    u8 pad_0x68[0x7c];
    f32 fov;
};

bool fn_71010e54f4(void* camera, UnkCamViewBox_l0e54f4* box) {
    UnkCamComponent_l0e54f4* self = static_cast<UnkCamComponent_l0e54f4*>(camera);
    if (!self->nodes)
        return false;
    UnkCamTarget_l0e54f4* target = self->nodes->value;
    if (!target)
        return false;
    UnkVec3f position = target->position;
    f32 z = position.z;
    f32 half_fov = static_cast<f32>(self->fov * 3.14159265358979323846 / 180.0) * 0.5f;
    f32 half_width = tanf(half_fov) * z;
    f32 scale = fn_7100132fa0();
    f32 adjusted = fn_7100217ee0(static_cast<f32>(half_fov * 180.0f / 3.14159265358979323846), scale);
    f32 half_height = tanf(static_cast<f32>(adjusted * 3.14159265358979323846 / 180.0)) * z;
    base::math::CVector2D center;
    fn_710007b430(&center, reinterpret_cast<const base::math::CVector3D*>(&position));
    box->minX = center.x - half_width;
    box->minY = center.y - half_height;
    box->maxX = half_width + center.x;
    box->maxY = half_height + center.y;
    return true;
}
