// CSpotLightComponent's unit (tools/tu_map.py): after the base update, hands the owner's position to the
// component (its virtual at +0x168) when it moved by more than 0.001, and points the light along x rotated
// by the owner's angles when they moved by as much. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CComponent.h"
#include "game/UnkTU251.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMat4f;

struct UnkSpotLightData_l8f8f98 {
    u8 pad_0x0[0x30];
    UnkVec3f direction;  // 0x30
    u8 pad_0x3c[0x1b8 - 0x3c];
    u8 dirty;  // 0x1b8
};

struct UnkSpotLightNode_l8f8f98 {
    u8 pad_0x0[0x78];
    UnkSpotLightData_l8f8f98* data;  // 0x78
};

struct UnkSpotLightObj_l8f8f98 {
    u8 pad_0x0[0x138];
    UnkSpotLightNode_l8f8f98* node;  // 0x138
};

struct UnkSpotLightOwner_l8f8f98 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkSpotLightComponent_l8f8f98;

struct UnkSpotLightVtable_l8f8f98 {
    void* pad_0x0[0x168 / 8];
    void (*SetPosition)(UnkSpotLightComponent_l8f8f98* self, const UnkVec3f* position);  // 0x168
};

struct UnkSpotLightComponent_l8f8f98 {
    UnkSpotLightVtable_l8f8f98* vtable;
    u8 pad_0x8[0x28];
    UnkSpotLightOwner_l8f8f98* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkVec3f position;               // 0x40
    f32 angle[3];                    // 0x4c
    UnkSpotLightObj_l8f8f98* light;  // 0x58
};
static_assert(__builtin_offsetof(UnkSpotLightComponent_l8f8f98, light) == 0x58, "layout");

// Whether the angle `a` is within 0.001 of `b`.
static inline bool Near(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

// `v` through the matrix `m`.
static inline UnkVec3f Rotate(const UnkMatrix44& m, const UnkVec3f& v) {
    return fn_7100078170(reinterpret_cast<const UnkMat4f*>(&m), &v);
}

void fn_71008f8f98(UnkSpotLightComponent_l8f8f98* self) {
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
        UnkSpotLightNode_l8f8f98* node = self->light->node;
        UnkSpotLightData_l8f8f98* data = node->data;
        data->direction = direction;
        data->dirty = 1;
        fn_71001bdc3c(node);
    }
}
