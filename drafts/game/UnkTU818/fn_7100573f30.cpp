// Unit 818: draws an enemy's vision cone. When its owner can act and the cone is shown, casts 41 rays
// across the cone's arc against the collision tree (unless the owner's view ignores walls), builds a fan
// of triangles from where they stop and loads it into the two cone models; otherwise hides them. Then
// reports a change of the first model's visibility. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU140.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU252.h"
#include "game/UnkTU365.h"
#include "game/UnkTU39.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <math.h>

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;

void MakeZRotationMatrix(f32 angle, UnkMatrix44* out);
void fn_7100148148(UnkSceneObject* self, u32 layer, const UnkSceneSpan& a, const UnkSceneSpan& b);
void fn_7100149474(UnkSceneObject* object, const UnkVec3f* angle);
struct UnkEmmyAI_b63a3;
bool fn_7100551ce4(UnkEmmyAI_b63a3* self);

// As src/game/UnkTU140/UnkTU140_GrowAt_ovb0243.cpp defines it.
typedef u32 __attribute__((may_alias)) a_u32_9230;
struct UnkRecord16_9230 {
    u32 words[4];

    UnkRecord16_9230() : words{0, 0, 0, 0} {}
    UnkRecord16_9230(UnkRecord16_9230&& o) {
        ((a_u32_9230*)this)[0] = ((const a_u32_9230*)&o)[0];
        ((a_u32_9230*)this)[1] = ((const a_u32_9230*)&o)[1];
        ((a_u32_9230*)this)[2] = ((const a_u32_9230*)&o)[2];
        ((a_u32_9230*)this)[3] = ((const a_u32_9230*)&o)[3];
    }
};
UnkRecord16_9230* fn_7100149230(base::global::CRntVector<UnkRecord16_9230>* self, u32 index);

// As src/game/UnkTU437/UnkTU437_SegmentHit_l3523bc.cpp defines them: the segment query's hit, its
// callback object and the segment.
struct UnkHitRefA_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefA_ovb0198u() : ptr(nullptr) {}
    UnkHitRefA_ovb0198u(const UnkHitRefA_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefA_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefA_ovb0198u& operator=(const UnkHitRefA_ovb0198u& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};
struct UnkHitRefB_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefB_ovb0198u() : ptr(nullptr) {}
    UnkHitRefB_ovb0198u(const UnkHitRefB_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefB_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefB_ovb0198u& operator=(const UnkHitRefB_ovb0198u& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

struct UnkHitVec_ovb0198u {
    f32 x, y, z;

    UnkHitVec_ovb0198u() {}
    UnkHitVec_ovb0198u(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec_ovb0198u(const UnkHitVec_ovb0198u& o) : x(o.x), y(o.y), z(o.z) {}
    UnkHitVec_ovb0198u& operator=(const UnkHitVec_ovb0198u& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkCollisionHit_ovb0198u {
    UnkHitRefA_ovb0198u object;
    UnkHitRefB_ovb0198u field_0x8;
    s32 id;
    UnkHitVec_ovb0198u point;
    UnkHitVec_ovb0198u field_0x20;
    UnkHitVec_ovb0198u normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_ovb0198u()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

struct UnkSegmentHitQuery_ovb0198u {
    void** vtable;
    UnkCollisionHit_ovb0198u hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_ovb0198u(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_ovb0198u() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkHitSegment_ovb0198u {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_ovb0198u(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The owner's AI; when it is an E.M.M.I.'s, the byte at +0x1fc of its vf_0xd8 says the view ignores walls.
struct UnkConeViewState_l573f30 {
    u8 pad_0x0[0x1fc];
    bool field_0x1fc;
};
struct UnkConeAI_l573f30 : base::reflection::UnkReflObject {
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkConeViewState_l573f30* vf_0xd8();
};

struct UnkConeOwner_l573f30 {
    u8 pad_0x0[0x10];
    UnkConeAI_l573f30* ai;
};

struct UnkSceneMaterial_l573f30 {
    u8 pad_0x0[0x48];
    u32 field_0x48;
};
struct UnkSceneSubView_l573f30 {
    u8 pad_0x0[0xe8];
    UnkSceneMaterial_l573f30* material;
};

struct UnkVisionCone_l573f30 {
    u8 pad_0x0[0x8];
    UnkSceneObject* model;  // 0x8
    UnkSceneObject* edge;  // 0x10
    u32 modelLayer;  // 0x18
    u32 edgeLayer;  // 0x1c
    UnkConeOwner_l573f30* owner;  // 0x20
    u8 pad_0x28[0x1];
    bool field_0x29;
    u8 pad_0x2a[0x2];
    f32 red;  // 0x2c
    f32 green;  // 0x30
    f32 blue;  // 0x34
    u8 pad_0x38[0x4];
    f32 radius;  // 0x3c
    f32 arc;  // 0x40
    u8 pad_0x44[0x4];
    CVector2D center;  // 0x48
    CVector2D dir;  // 0x50
    u8 pad_0x58[0x8];
    bool field_0x60;
    u8 pad_0x61[0xf];
    CFilePathStrId edgePath;  // 0x70
    bool field_0x78;
    bool visible;  // 0x79
    u8 pad_0x7a[0x6];
    base::UnkDeferredSet<base::UnkDeferredCallback<bool>> onVisible;  // 0x80
};

// A colour with its negative components raised to zero.
inline CVector4D Clamped_l573f30(const CVector4D& c) {
    return CVector4D{c.x <= 0.0f ? 0.0f : c.x, c.y <= 0.0f ? 0.0f : c.y, c.z <= 0.0f ? 0.0f : c.z,
                     c.w <= 0.0f ? 0.0f : c.w};
}

inline void SetColor_l573f30(UnkSceneObjectSub* sub, u32 index, CVector4D value) {
    fn_71001bfea4(reinterpret_cast<UnkLockonModel_ovb0092*>(sub), index, reinterpret_cast<const UnkColor_ovb0092*>(&value));
}

inline void SetTint_l573f30(UnkSceneObjectSub* sub, u32 index, CVector4D value) {
    fn_71001bff28(reinterpret_cast<UnkLockonModel_ovb0092*>(sub), index, reinterpret_cast<UnkColor_ovb0092*>(&value));
}

inline void Hide_l573f30(UnkSceneObject* model) {
    fn_71001be430(model->field_0x130, 0, false);
    fn_71001be430(model->field_0x130, 1, false);
    fn_7100149490(model, false);
}

void fn_7100573f30(UnkVisionCone_l573f30* self) {
    const bool shown = self->field_0x60 || self->field_0x29;
    if (!self->edge && self->edgePath.data != &g_unk_7101d47200->empty) {
        CFilePathStrId path = self->edgePath;
        fn_71001494f8(&self->edge, 0x2a, 3, &path, 2);
    }
    if (fn_7100551ce4(reinterpret_cast<UnkEmmyAI_b63a3*>(self->owner)) && shown && self->field_0x78) {
        UnkMatrix44 rotation = {{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 1.0f}};
        UnkVec3f position;
        CStrId head("C01_Head_SKN", true);
        const f32 angle = atan2f(self->dir.y, self->dir.x);
        MakeZRotationMatrix(-angle, &rotation);
        position = UnkVec3f(self->center.x, self->center.y, 0.0f);
        CVector4D color = {self->red, self->green, self->blue, 0.1f};
        if (self->field_0x60) {
            CVector4D highlight = {0.8f, 0.8f, 1.0f, 0.5f};
            fn_710007b518(&color, &highlight);
        }
        const f32 start =
            fn_7100030dec(reinterpret_cast<UnkFloatList_b46a1*>(reinterpret_cast<u8*>(self->owner) + 0x2a448)) +
            (atan2f(self->dir.x, self->dir.y) - self->arc * 0.5f);
        const f32 step = self->arc / 40.0f;

        CRntVector<UnkVec3f> vertices(0xf);
        vertices.Reserve(42);
        vertices.PushBackCopy(reinterpret_cast<const UnkVec3f&>(g_unk_710164779c));
        UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
        UnkTree* tree = world ? static_cast<UnkTree*>(world->field_0x820) : nullptr;
        for (u32 i = 0; i <= 40; i++) {
            const f32 a = start + step * i;
            f32 dx = sinf(a) * self->radius;
            f32 dy = cosf(a) * self->radius;
            UnkHitSegment_ovb0198u segment(self->center, CVector2D{self->center.x + dx, self->center.y + dy});
            UnkConeAI_l573f30* ai = self->owner->ai;
            if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710051e010()) || !ai->vf_0xd8()->field_0x1fc) {
                UnkSegmentHitQuery_ovb0198u query(CVector3D{self->center.x, self->center.y, 0.0f});
                if (fn_7100d5c518(tree, 3, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
                    CVector2D hit;
                    fn_710007b430(&hit, reinterpret_cast<const CVector3D*>(&query.hit.point));
                    dy = hit.y - self->center.y;
                    dx = hit.x - self->center.x;
                }
            }
            UnkVec3f local(self->center.x + dx - position.x, self->center.y + dy - position.y, 0.0f - position.z);
            vertices.PushBackCopy(TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&rotation), &local));
        }

        CRntVector<u16> indices(0xf);
        CRntVector<UnkRecord16_9230> colors(0xf);
        colors.Reserve(42);
        for (u32 i = 0; i < 42; i++)
            fn_710007b518(reinterpret_cast<CVector4D*>(fn_7100149230(&colors, i)), &color);
        indices.Reserve(120);
        for (u16 i = 1; i <= 40; i++) {
            indices.PushBack(0);
            indices.PushBack(i);
            indices.PushBack(i + 1);
        }

        // Load the triangles into the cone model and show it with the cone's colour.
        if (self->model) {
            fn_71001be430(self->model->field_0x130, 0, false);
            fn_71001be430(self->model->field_0x130, 1, false);
            self->modelLayer = self->modelLayer == 0;
            fn_7100148148(self->model, self->modelLayer, UnkSceneSpan{vertices.data, vertices.count}, UnkSceneSpan{colors.data, colors.count});
            fn_7100148178(self->model, self->modelLayer, UnkSceneSpan{indices.data, indices.count}, 1);
            reinterpret_cast<UnkSceneSubView_l573f30*>(self->model->field_0x130)->material->field_0x48 = 2;
            fn_71001beee4(self->model->field_0x130, color.w < 1.0f);
            SetColor_l573f30(self->model->field_0x130, 0, Clamped_l573f30(color));
            SetTint_l573f30(self->model->field_0x130, 1, CVector4D{1.0f, 1.0f, 1.0f, 1.0f});
            SetColor_l573f30(self->model->field_0x130, 1, CVector4D{self->arc, self->radius, 1.0f, 1.0f});
            fn_7100149460(self->model, &position);
            UnkVec3f facing(0.0f, 0.0f, angle);
            fn_7100149474(self->model, &facing);
            fn_71001be430(self->model->field_0x130, self->modelLayer, true);
            fn_7100149490(self->model, true);
        }
        // The same for the edge model.
        if (self->edge) {
            fn_71001be430(self->edge->field_0x130, 0, false);
            fn_71001be430(self->edge->field_0x130, 1, false);
            self->edgeLayer = self->edgeLayer == 0;
            fn_7100148148(self->edge, self->edgeLayer, UnkSceneSpan{vertices.data, vertices.count}, UnkSceneSpan{colors.data, colors.count});
            fn_7100148178(self->edge, self->edgeLayer, UnkSceneSpan{indices.data, indices.count}, 1);
            reinterpret_cast<UnkSceneSubView_l573f30*>(self->edge->field_0x130)->material->field_0x48 = 2;
            fn_71001beee4(self->edge->field_0x130, color.w < 1.0f);
            SetColor_l573f30(self->edge->field_0x130, 0, Clamped_l573f30(color));
            SetTint_l573f30(self->edge->field_0x130, 1, CVector4D{1.0f, 1.0f, 1.0f, 1.0f});
            SetColor_l573f30(self->edge->field_0x130, 1, CVector4D{self->arc, self->radius, 1.0f, 1.0f});
            fn_7100149460(self->edge, &position);
            UnkVec3f facing(0.0f, 0.0f, angle);
            fn_7100149474(self->edge, &facing);
            fn_71001be430(self->edge->field_0x130, self->edgeLayer, true);
            fn_7100149490(self->edge, true);
        }
        colors.Free();
        indices.Free();
        vertices.Free();
    } else {
        if (self->model)
            Hide_l573f30(self->model);
        if (self->edge)
            Hide_l573f30(self->edge);
    }
    const bool visible = self->model ? self->model->field_0x20[0xf8] != 0 : false;
    if (visible != self->visible) {
        self->visible = visible;
        fn_71000303c0(&self->onVisible, &self->visible);
    }
}
