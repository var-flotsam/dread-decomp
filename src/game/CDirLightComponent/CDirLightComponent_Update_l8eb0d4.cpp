// CDirLightComponent's unit (tools/tu_map.py): after the base update, hands the owner's position to the
// component (its virtual at +0x168) when it moved by more than 0.001, and points the light along x rotated
// by the owner's angles when they moved by as much. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CComponent.h"
#include "game/UnkTU251.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMat4f;

struct UnkDirLightData_l8eb0d4 {
    u8 pad_0x0[0x30];
    UnkVec3f direction;  // 0x30
    u8 pad_0x3c[0x1b8 - 0x3c];
    u8 dirty;  // 0x1b8
};

struct UnkDirLightNode_l8eb0d4 {
    u8 pad_0x0[0x78];
    UnkDirLightData_l8eb0d4* data;  // 0x78
};

struct UnkDirLightObj_l8eb0d4 {
    u8 pad_0x0[0x138];
    UnkDirLightNode_l8eb0d4* node;  // 0x138
};

struct UnkDirLightOwner_l8eb0d4 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkDirLightComponent_l8eb0d4;

struct UnkDirLightVtable_l8eb0d4 {
    void* pad_0x0[0x168 / 8];
    void (*SetPosition)(UnkDirLightComponent_l8eb0d4* self, const UnkVec3f* position);  // 0x168
};

struct UnkDirLightComponent_l8eb0d4 {
    UnkDirLightVtable_l8eb0d4* vtable;
    u8 pad_0x8[0x28];
    UnkDirLightOwner_l8eb0d4* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkVec3f position;              // 0x40
    f32 angle[3];                   // 0x4c
    UnkDirLightObj_l8eb0d4* light;  // 0x58
};
static_assert(__builtin_offsetof(UnkDirLightComponent_l8eb0d4, light) == 0x58, "layout");

// Whether the angle `a` is within 0.001 of `b`.
static inline bool Near(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

// `v` through the matrix `m`.
static inline UnkVec3f Rotate(const UnkMatrix44& m, const UnkVec3f& v) {
    return fn_7100078170(reinterpret_cast<const UnkMat4f*>(&m), &v);
}

void fn_71008eb0d4(UnkDirLightComponent_l8eb0d4* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkVec3f position = self->owner->xform.GetTranslation();
    if (UNK_CHANGED(position.x, self->position.x) || UNK_CHANGED(position.y, self->position.y) ||
        UNK_CHANGED(position.z, self->position.z)) {
        self->vtable->SetPosition(self, &position);
        self->position = position;
    }
    UnkVec3f angle = self->owner->xform.GetAngle();
    if (!Near(angle.x, self->angle[0]) || !Near(angle.y, self->angle[1]) || !Near(angle.z, self->angle[2])) {
        self->angle[0] = angle.x;
        self->angle[1] = angle.y;
        self->angle[2] = angle.z;
        UnkVec3f direction =
            Rotate(fn_7100078fc8(reinterpret_cast<const UnkMatrix44&>(angle)), UnkVec3f(1.0f, 0.0f, 0.0f));
        UnkDirLightNode_l8eb0d4* node = self->light->node;
        UnkDirLightData_l8eb0d4* data = node->data;
        data->direction = direction;
        data->dirty = 1;
        fn_71001bdc3c(node);
    }
}
