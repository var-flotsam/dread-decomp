// CSegmentLightComponent's unit (tools/tu_map.py): after the base update, hands the owner's position to the
// component (its virtual at +0x168) when it moved by more than 0.001, and when the owner's angles moved by as
// much, points the light's segment along x rotated by them, keeping its length (at least 1). Names and
// layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CComponent.h"
#include "game/UnkTU251.h"
#include "game/UnkTU54.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMat4f;

struct UnkSegmentLightData_l8f404c {
    u8 pad_0x0[0x30];
    UnkVec3f segment;  // 0x30
    u8 pad_0x3c[0x1b8 - 0x3c];
    u8 dirty;  // 0x1b8
};

struct UnkSegmentLightNode_l8f404c {
    u8 pad_0x0[0x78];
    UnkSegmentLightData_l8f404c* data;  // 0x78
};

struct UnkSegmentLightObj_l8f404c {
    u8 pad_0x0[0x138];
    UnkSegmentLightNode_l8f404c* node;  // 0x138
};

struct UnkSegmentLightOwner_l8f404c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkSegmentLightComponent_l8f404c;

struct UnkSegmentLightVtable_l8f404c {
    void* pad_0x0[0x168 / 8];
    void (*SetPosition)(UnkSegmentLightComponent_l8f404c* self, const UnkVec3f* position);  // 0x168
};

struct UnkSegmentLightComponent_l8f404c {
    UnkSegmentLightVtable_l8f404c* vtable;
    u8 pad_0x8[0x28];
    UnkSegmentLightOwner_l8f404c* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkVec3f position;                  // 0x40
    f32 angle[3];                       // 0x4c
    UnkSegmentLightObj_l8f404c* light;  // 0x58

    const UnkVec3f& Segment() const { return light->node->data->segment; }
};
static_assert(__builtin_offsetof(UnkSegmentLightComponent_l8f404c, light) == 0x58, "layout");

// Whether the angle `a` is within 0.001 of `b`.
static inline bool Near(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

// `v` through the matrix `m`.
static inline UnkVec3f Rotate(const UnkMatrix44& m, const UnkVec3f& v) {
    return TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&m), &v);
}

void CSegmentLightComponent_SyncPositionAndSegmentDirFromOwner(UnkSegmentLightComponent_l8f404c* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkVec3f position = self->owner->xform.GetTranslation();
    if (UNK_CHANGED(position.x, self->position.x) || UNK_CHANGED(position.y, self->position.y) ||
        UNK_CHANGED(position.z, self->position.z)) {
        self->vtable->SetPosition(self, &position);
        self->position = position;
    }
    UnkVec3f angle = self->owner->xform.GetAngle();
    if (!Near(angle.x, self->angle[0]) || !Near(angle.y, self->angle[1]) || !Near(angle.z, self->angle[2])) {
        f32 length = Length(Normalized(self->Segment()));
        self->angle[0] = angle.x;
        self->angle[1] = angle.y;
        self->angle[2] = angle.z;
        UnkVec3f rotated =
            Rotate(fn_7100078fc8(angle), UnkVec3f(1.0f, 0.0f, 0.0f));
        UnkVec3f segment = rotated * length;
        UnkSegmentLightObj_l8f404c* light = self->light;
        segment = Normalized(segment);
        f32 scale = Length(self->Segment()) < 1.0f ? 1.0f : Length(self->Segment());
        segment = segment * scale;
        UnkSegmentLightNode_l8f404c* node = light->node;
        UnkSegmentLightData_l8f404c* data = node->data;
        data->segment = segment;
        data->dirty = 1;
        fn_71001bdc3c(node);
    }
}
