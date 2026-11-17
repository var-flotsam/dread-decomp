// Unit 274 (tools/tu_map.py, no reliable name): sets a transform node's scale when it changed by more
// than 0.001, passes the new matrix to the target's copy (+0x78, matrix at +0x28) and rescales the
// node's sphere (+0x80) by the ratio of the new largest scale to the old. Names and layouts are placeholders.
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"

struct UnkScaledModel_l1f47ac {
    u8 pad_0x0[0x28];
    UnkMatrix44 matrix;
};

struct UnkSphereXformNode_l1f47ac {
    u8 pad_0x0[0x18];
    UnkXform xform;  // 0x18
    u8 pad_0x74[0xc];
    UnkSphere sphere;  // 0x80
    u8 pad_0xd0[0x38];
    UnkXformTarget* target;  // 0x108
};
static_assert(__builtin_offsetof(UnkSphereXformNode_l1f47ac, target) == 0x108, "layout");

static inline const f32& Max(const f32& a, const f32& b) {
    return a < b ? b : a;
}

static inline f32 MaxScale(UnkVec3f s) {
    return Max(Max(s.x, s.y), s.z);
}

void fn_71001f47ac(UnkSphereXformNode_l1f47ac* self, const UnkVec3f* v) {
    UnkXform* x = &self->xform;
    UnkVec3f cur = x->GetScale();
    if (UNK_CHANGED(cur.x, v->x) || UNK_CHANGED(cur.y, v->y) || UNK_CHANGED(cur.z, v->z)) {
        f32 oldMax = MaxScale(x->GetScale());
        x->SetScale(*v);
        UnkXformTarget* target = self->target;
        const UnkMatrix44& m = x->GetMatrix();
        UnkCopyMatrix(&static_cast<UnkScaledModel_l1f47ac*>(target->field_0x78)->matrix, m);
        f32 radius = self->sphere.radius;
        fn_710021693c(&self->sphere, radius * (MaxScale(x->GetScale()) / oldMax));
    }
}
