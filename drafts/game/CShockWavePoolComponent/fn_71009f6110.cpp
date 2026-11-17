// CShockWavePoolComponent unit. fn_71009f6110: starts the pool from its parameters: drops the start
// point onto the floor below, offsets it along the segment's normal, places the owner there, and loads
// the pool's model turned to the floor's slope with its two extents set as shader parameters; on a
// floor too steep (or a refused start) it hides the model instead. Names and layouts are placeholders.
#include <math.h>
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/math/UnkSegment2D.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU108.h"
#include "game/UnkTU127.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU341.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;

struct UnkShockWavePoolMember;
void fn_71009f5d50(UnkShockWavePoolMember* self, const UnkShockWavePoolMember* other);  // assignment
void fn_71001435b4(UnkModel_ovb0032* self, const UnkVec3f* angle);

// The pool's parameters (the member at +0x40 of the component).
struct UnkShockWavePoolParams {
    u8 pad_0x0[0x10c];
    UnkVec3f start;  // 0x10c
    bool drop_to_floor;  // 0x118
    u8 pad_0x119[0x3];
    f32 field_0x11c;
    f32 normal_offset;  // 0x120
    u8 pad_0x124[0x4];
    f32 field_0x128;
    u8 pad_0x12c[0x150 - 0x12c];
    base::global::CFilePathStrId model_path;  // 0x150
    u8 pad_0x158[0x20c - 0x158];
    bool check_slope;  // 0x20c
    u8 pad_0x20d[0x3];
    UnkSegment2D segment;  // 0x210
};
static_assert(sizeof(UnkShockWavePoolParams) == 0x228, "params layout");

// A reference-counted object a hit holds.
struct UnkShockWavePoolRef {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();
    u32 ref_count;
};

struct UnkShockWavePoolRefSlot {
    UnkShockWavePoolRef* ptr;
    UnkShockWavePoolRefSlot() : ptr(nullptr) {}
    ~UnkShockWavePoolRefSlot() {
        if (ptr && --ptr->ref_count == 0)
            ptr->Release();
    }
};

// A floor query's hit: two references, an id, then the point and the rest of the contact.
struct UnkShockWavePoolHit {
    UnkShockWavePoolRefSlot slot_a;
    UnkShockWavePoolRefSlot slot_b;
    u32 id;
    UnkVec3f point;
    u32 field_0x20[9];

    UnkShockWavePoolHit() : id(0xffffffff), point(0.0f, 0.0f, 0.0f) {
        for (u32 i = 0; i < 9; i++)
            field_0x20[i] = 0;
    }
};

struct UnkShockWavePoolParam {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkShockWavePoolMaterial {
    u8 pad_0x0[0x14c];
    CVector4D field_0x14c;
    u8 pad_0x15c[0x17c - 0x15c];
    CVector4D field_0x17c;
    u8 pad_0x18c[0x1f0 - 0x18c];
    UnkShockWavePoolParam* field_0x1f0;
    u8 pad_0x1f8[0x208 - 0x1f8];
    UnkShockWavePoolParam* field_0x208;
    u8 pad_0x210[0x240 - 0x210];
    CVector4D field_0x240;
    u8 pad_0x250[0x270 - 0x250];
    CVector4D field_0x270;
    u8 pad_0x280[0x2e0 - 0x280];
    UnkShockWavePoolParam* field_0x2e0;
    u8 pad_0x2e8[0x2f8 - 0x2e8];
    UnkShockWavePoolParam* field_0x2f8;
};

struct UnkShockWavePoolShader {
    u8 pad_0x0[0x48];
    UnkShockWavePoolMaterial** material;  // 0x48
};

struct UnkShockWavePoolModelData {
    u8 pad_0x0[0x78];
    UnkShockWavePoolShader* shader;  // 0x78
};

struct UnkShockWavePoolModel {
    void* vtable;
    u8 node[0x30];  // 0x8, the scene node the render manager takes
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x138 - 0x94];
    UnkShockWavePoolModelData* data;  // 0x138
};

struct UnkShockWavePoolOwner;
struct UnkShockWavePoolOwnerVtable {
    u8 pad_0x0[0xd0];
    void (*vf_0xd0)(UnkShockWavePoolOwner* self, const UnkVec3f* position, bool flag);
};
struct UnkShockWavePoolOwner {
    const UnkShockWavePoolOwnerVtable* vtable;
};

// The pool's outline, which fn_7100118b94 adds points to.
struct UnkShockWavePoolOutline {
    u8 pad_0x0[0x8];
    u32 field_0x8;
    u8 pad_0xc[0x28 - 0xc];
    u32 field_0x28;
};

struct UnkShockWavePoolStart {
    u8 pad_0x0[0x30];
    UnkShockWavePoolOwner* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkShockWavePoolParams params;  // 0x40
    u8 pad_0x268[0x2a0 - 0x268];
    f32 field_0x2a0;
    u8 pad_0x2a4[0x4];
    UnkShockWavePoolOutline outline;  // 0x2a8
    bool field_0x2d4;
    bool field_0x2d5;
    u8 pad_0x2d6[0x2f0 - 0x2d6];
    CVector2D position_2d;  // 0x2f0
    u8 pad_0x2f8[0x310 - 0x2f8];
    bool field_0x310;
    bool field_0x311;
    u8 pad_0x312[0x2];
    u32 field_0x314;
    UnkShockWavePoolModel* model;  // 0x318
    u8 pad_0x320[0x4];
    bool field_0x324;
};
static_assert(__builtin_offsetof(UnkShockWavePoolStart, field_0x324) == 0x324, "layout");

void fn_71009f6750(UnkShockWavePoolStart* self, const CVector2D* position, const CVector2D* start,
                   const UnkVec2Result* normal, bool* ok);

// How upright a normal is: its dot product with the up axis.
inline f32 Upness(const UnkVec2Result& n) {
    return n.x * 0.0f + n.y * 1.0f;
}

inline f32 SafeAcos(f32 c) {
    if (c >= 1.0f)
        return 0.0f;
    if (c <= -1.0f)
        return 3.14159274f;
    return acosf(c);
}
// The point's position in the plane.
inline void Flatten(CVector2D* out, const UnkVec3f& v) {
    fn_710007b430(out, reinterpret_cast<const CVector3D*>(&v));
}

inline CVector2D Flat(const UnkVec3f& v) {
    CVector2D out;
    Flatten(&out, v);
    return out;
}

// Copies a point in the plane coordinate by coordinate.
inline void Copy2D(CVector2D* to, const CVector2D& from) {
    to->x = from.x;
    to->y = from.y;
}

// Adds a point of the plane to the pool's outline.
inline void AddOutlinePoint(UnkShockWavePoolOutline* outline, const CVector2D& p) {
    UnkVec3f point(p.x, p.y, 0.0f);
    fn_7100118b94(outline, reinterpret_cast<UnkVec3*>(&point));
}

// The segment's slope: the angle between its normal and the up axis.
inline f32 SlopeAngle(const UnkSegment2D* segment) {
    UnkVec2Result n = fn_7100125e78(segment);
    UnkVec3f n3(n.x, n.y, 0.0f);
    return SafeAcos(Dot(n3, UnkVec3f(0.0f, 1.0f, 0.0f)) / Length(n3));
}

inline f32 Distance2D(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}


inline UnkShockWavePoolMaterial* Material(UnkShockWavePoolModel* model) {
    return *model->data->shader->material;
}

inline void SetParam(CVector4D& field, UnkShockWavePoolParam* const& param, const CVector4D& value) {
    fn_710007b518(&field, &value);
    if (param)
        *param->value = value;
}

inline UnkSceneNode* Node(UnkShockWavePoolModel* model) {
    return model ? reinterpret_cast<UnkSceneNode*>(model->node) : nullptr;
}

inline UnkItemBase_71011a91c4* Item(UnkShockWavePoolModel* model) {
    return model ? reinterpret_cast<UnkItemBase_71011a91c4*>(model->node) : nullptr;
}

static void Hide(UnkShockWavePoolStart* self) {
    if (self->model) {
        fn_71001436a0(reinterpret_cast<UnkModel*>(self->model), 0);
        fn_710014364c(reinterpret_cast<UnkModel*>(self->model), 0);
        fn_71002199d8(g_unk_7101d47300, Item(self->model), 0);
        self->model = nullptr;
    }
    fn_7100232c68(self->owner);
}

void fn_71009f6110(UnkShockWavePoolStart* self, const UnkShockWavePoolParams* params) {
    fn_71009f5d50(reinterpret_cast<UnkShockWavePoolMember*>(&self->params),
                  reinterpret_cast<const UnkShockWavePoolMember*>(params));
    if (self->params.check_slope && Upness(fn_7100125e78(&self->params.segment)) < 0.7f) {
        Hide(self);
        return;
    }
    self->field_0x2a0 = 0.0f;
    if (self->params.field_0x128 != 0.0f)
        self->field_0x2a0 = self->params.field_0x11c / self->params.field_0x128;
    self->field_0x2d4 = false;
    self->field_0x2d5 = true;
    UnkShockWavePoolOutline* outline = &self->outline;
    outline->field_0x8 = 0;
    outline->field_0x28 = 0;

    CVector2D start;
    UnkVec3f pos = self->params.start;
    if (self->params.drop_to_floor) {
        UnkVec3f below(pos.x, pos.y - 1500.0f, pos.z);
        UnkShockWavePoolHit hit;
        if (fn_7100d5b714(g_unk_7101d47310->field_0x608->field_0x820, 1, &pos, &below,
                          reinterpret_cast<UnkFloorRef_ovb0048*>(&hit), true, false, 0.1f))
            pos = hit.point;
    }
    Flatten(&start, pos);
    if (self->params.normal_offset > 0.0f && Upness(fn_7100125e78(&self->params.segment)) > 0.7f) {
        UnkVec2Result n = fn_7100125e78(&self->params.segment);
        pos = UnkVec3f(n.x, n.y, 0.0f) * self->params.normal_offset + pos;
    }
    Copy2D(&self->position_2d, Flat(pos));
    self->owner->vtable->vf_0xd0(self->owner, &pos, true);

    CVector2D near_end;
    Flatten(&near_end, pos);
    CVector2D far_end;
    Flatten(&far_end, pos);
    AddOutlinePoint(outline, near_end);
    AddOutlinePoint(outline, far_end);
    self->field_0x310 = false;
    self->field_0x311 = false;

    bool ok = true;
    {
        CVector2D position;
        Copy2D(&position, self->position_2d);
        UnkVec2Result normal = fn_7100125e78(&self->params.segment);
        fn_71009f6750(self, &position, &start, &normal, &ok);
    }
    if (!ok) {
        Hide(self);
        return;
    }
    self->field_0x314 = 0;
    if (self->params.model_path.data != &g_unk_7101d47200->empty) {
        UnkRenderManager* manager = g_unk_7101d47300;
        {
            UnkShockWavePoolModel* model = nullptr;
            fn_71006cae2c(manager->field_0x218, self->params.model_path, reinterpret_cast<UnkModel**>(&model),
                          &g_unk_7101d470e0);
            fn_7100219ae0(manager, Node(model), 0);
            self->model = model;
        }

        f32 slope = SlopeAngle(&self->params.segment);
        if (fn_7100125e78(&self->params.segment).x < 0.0f)
            slope = -slope;
        UnkVec3f angle = self->model->xform.GetAngle();
        angle.z -= slope;
        fn_7100143578(reinterpret_cast<UnkModel_ovb0032*>(self->model), &pos);
        fn_71001435b4(reinterpret_cast<UnkModel_ovb0032*>(self->model), &angle);

        CVector4D extent;
        {
            CVector2D now;
            Flatten(&now, pos);
            extent.x = Distance2D(far_end, now);
            Flatten(&now, pos);
            extent.y = -Distance2D(near_end, now);
        }
        extent.z = 0.0f;
        extent.w = 0.0f;
        const CVector4D& tint = Material(self->model)->field_0x270;
        CVector4D color;
        color.x = tint.x;
        color.y = tint.y;
        color.z = tint.z;
        color.w = 1.0f;
        UnkShockWavePoolMaterial* m = Material(self->model);
        SetParam(m->field_0x240, m->field_0x2e0, extent);
        m = Material(self->model);
        SetParam(m->field_0x14c, m->field_0x1f0, extent);
        m = Material(self->model);
        SetParam(m->field_0x270, m->field_0x2f8, color);
        m = Material(self->model);
        SetParam(m->field_0x17c, m->field_0x208, color);

        fn_71001436a0(reinterpret_cast<UnkModel*>(self->model), 1);
        fn_710014364c(reinterpret_cast<UnkModel*>(self->model), 1);
    }
    self->field_0x324 = false;
}