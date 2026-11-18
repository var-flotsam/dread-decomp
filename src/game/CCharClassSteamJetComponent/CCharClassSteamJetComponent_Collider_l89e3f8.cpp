// CCharClassSteamJetComponent unit: creates the steam jet's collider once (unless +0xc08 says not to) and
// sizes its box from the jet's length (+0xbe4) and width (+0xbe8), laid along the way the owner's up axis
// points (up, down, right or left), then enables it. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

using UnkSteamJetCreateCollider_l89e3f8 = UnkCollisionObject* (*)(UnkCollisionContainer*, const CStrId*,
                                                                   const CStrId*, const CStrId*, const CStrId*, u32,
                                                                   u32, u32, void*, s32, void*, s32, void*, s32,
                                                                   bool, bool, bool);

// A collider's shape; kind 4 (vf_0x18) is a box whose extent is at +0xa8.
struct UnkSteamJetShape_l89e3f8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
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

    u8 pad_0x8[0xa0];
    f32 left;    // +0xa8
    f32 bottom;  // +0xac
    f32 right;   // +0xb0
    f32 top;     // +0xb4
};

struct UnkSteamJetCollider_l89e3f8 {
    u8 pad_0x0[0x158];
    UnkSteamJetShape_l89e3f8* shape;  // +0x158
};

struct UnkSteamJetOwner_l89e3f8 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // +0x38
    u8 pad_0x98[0x2a0 - 0x38 - sizeof(UnkXform)];
    UnkCollisionContainer* collision;  // +0x2a0
};
static_assert(__builtin_offsetof(UnkSteamJetOwner_l89e3f8, collision) == 0x2a0, "UnkSteamJetOwner_l89e3f8 layout");

struct UnkSteamJetComponent_l89e3f8 {
    u8 pad_0x0[0x30];
    UnkSteamJetOwner_l89e3f8* owner;  // +0x30
    u8 pad_0x38[0xbe4 - 0x38];
    f32 length;  // +0xbe4
    f32 width;   // +0xbe8
    u8 pad_0xbec[0xc08 - 0xbec];
    bool no_collider;  // +0xc08
};
static_assert(__builtin_offsetof(UnkSteamJetComponent_l89e3f8, no_collider) == 0xc08,
              "UnkSteamJetComponent_l89e3f8 layout");

static inline const CStrId* SingletonId(u32 offset) {
    return reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + offset);
}

static inline UnkSteamJetShape_l89e3f8* Box(UnkSteamJetShape_l89e3f8* shape) {
    return shape->vf_0x18() == 4 ? shape : nullptr;
}

// The same with a check for a missing shape, which the original makes in the upward case only.
static inline UnkSteamJetShape_l89e3f8* CheckedBox(UnkSteamJetShape_l89e3f8* shape) {
    return shape && shape->vf_0x18() == 4 ? shape : nullptr;
}

static inline f32 Dot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// The owner's up axis: the matrix's second column.
static inline CVector3D UpAxis(UnkSteamJetOwner_l89e3f8* owner) {
    const UnkMatrix44& m = owner->xform.GetMatrix();
    CVector3D up;
    up.x = m.m[1];
    up.y = m.m[5];
    up.z = m.m[9];
    return up;
}

// Sets the box's extent and has it update.
static inline void SetBox(UnkSteamJetShape_l89e3f8* box, f32 left, f32 bottom, f32 right, f32 top) {
    box->left = left;
    box->bottom = bottom;
    box->right = right;
    box->top = top;
    box->vf_0x80();
}

void fn_710089e3f8(UnkSteamJetComponent_l89e3f8* self) {
    if (self->no_collider)
        return;
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return;
    const CStrId* name = SingletonId(0xb40);
    const CStrId* group = SingletonId(0xb40);
    const CStrId* type = SingletonId(0x628);
    UnkSteamJetCreateCollider_l89e3f8 create =
        (*reinterpret_cast<UnkSteamJetCreateCollider_l89e3f8**>(collision))[0x168 / 8];
    UnkCollisionObject* object = create(collision, name, group, &base::global::CStrId::Empty(), type, 8, 0, 1, nullptr, -1,
                                        nullptr, -1, nullptr, -1, false, false, true);
    if (!object)
        return;
    UnkSteamJetCollider_l89e3f8* collider = reinterpret_cast<UnkSteamJetCollider_l89e3f8*>(object);

    CVector2D pos;
    UnkVec3f translation = self->owner->xform.GetTranslation();
    fn_710007b430(&pos, reinterpret_cast<const CVector3D*>(&translation));
    f32 width = self->width - 10.0f;
    width = !(width < 1.0f) ? width : 1.0f;
    f32 length = self->length - 10.0f;
    length = !(length < 1.0f) ? length : 1.0f;

    CVector3D x_axis;
    x_axis.x = 1.0f;
    x_axis.y = 0.0f;
    x_axis.z = 0.0f;
    CVector3D y_axis;
    y_axis.x = 0.0f;
    y_axis.y = 1.0f;
    y_axis.z = 0.0f;
    f32 up = Dot(UpAxis(self->owner), y_axis);
    f32 right = Dot(UpAxis(self->owner), x_axis);

    if (up > 0.9f) {
        f32 half = width * 0.5f;
        f32 left = pos.x - half, bottom = pos.y, right_edge = half + pos.x, top = length + pos.y;
        SetBox(CheckedBox(collider->shape), left, bottom, right_edge, top);
    } else if (up < -0.9f) {
        f32 half = width * 0.5f;
        f32 left = pos.x - half, bottom = pos.y - length, right_edge = half + pos.x, top = pos.y + 0.0f;
        SetBox(Box(collider->shape), left, bottom, right_edge, top);
    } else if (right > 0.9f) {
        f32 half = width * 0.5f;
        f32 left = pos.x, bottom = pos.y - half, right_edge = length + pos.x, top = half + pos.y;
        SetBox(Box(collider->shape), left, bottom, right_edge, top);
    } else {
        f32 half = width * 0.5f;
        f32 left = pos.x - length, bottom = pos.y - half, right_edge = pos.x + 0.0f, top = half + pos.y;
        SetBox(Box(collider->shape), left, bottom, right_edge, top);
    }
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), false);
}
