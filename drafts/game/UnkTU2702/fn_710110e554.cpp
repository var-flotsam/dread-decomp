// Unit 2702 (CAIComponent?): follows the owner's anchor: the position comes from the owner (or the
// named shape's frame plus an offset), and while following, the owner is moved towards its target.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words CopyMatrixWithoutTranslation(const UnkMatrix44Words* src);

struct UnkAIFollowName {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkAIFollowAnchor {
    u8 pad_0x0[0x458];
    base::global::UnkStringData* shape;  // 0x458
    UnkVec3f offset;  // 0x460
};

struct UnkAIFollowOwner {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1c0];
    UnkEntity* shapes;  // 0x228
    u8 pad_0x230[0x180];
    bool moved;  // 0x3b0
};
static_assert(__builtin_offsetof(UnkAIFollowOwner, shapes) == 0x228, "UnkAIFollowOwner layout");
static_assert(__builtin_offsetof(UnkAIFollowOwner, moved) == 0x3b0, "UnkAIFollowOwner layout");

struct UnkAIFollow;
struct UnkAIFollowVtbl {
    u8 pad_0x0[0x420];
    bool (*vf_0x420)(UnkAIFollow* self);
};

struct UnkAIFollow {
    UnkAIFollowVtbl* vtable;
    u8 pad_0x8[0x28];
    UnkAIFollowOwner* owner;  // 0x30
    u8 pad_0x38[0x1580];
    CVector3D last;  // 0x15b8
    CVector3D pos;  // 0x15c4
    u8 pad_0x15d0[0x40];
    UnkAIFollowAnchor* anchor;  // 0x1610
    u8 pad_0x1618[0x13050];
    f32 time;  // 0x14668
};
static_assert(__builtin_offsetof(UnkAIFollow, last) == 0x15b8, "UnkAIFollow layout");
static_assert(__builtin_offsetof(UnkAIFollow, anchor) == 0x1610, "UnkAIFollow layout");
static_assert(__builtin_offsetof(UnkAIFollow, time) == 0x14668, "UnkAIFollow layout");

static inline CVector3D PosOf(const UnkAIFollowOwner* owner) {
    CVector3D v;
    v.x = owner->x;
    v.y = owner->y;
    v.z = owner->z;
    return v;
}

static inline CVector3D TranslationOf(const UnkMatrix44& m) {
    CVector3D v;
    v.x = m.m[3];
    v.y = m.m[7];
    v.z = m.m[11];
    return v;
}

static inline bool NearZero(f32 v) {
    return v + 0.001f >= 0.0f && v - 0.001f <= 0.0f;
}

// The frame of the shape named `shape`, found through its hash.
static inline bool FindShape(UnkEntity* shapes, base::global::UnkStringData* shape, UnkMatrix44* frame) {
    u64 key = reinterpret_cast<UnkAIFollowName*>(shape)->hash;
    return fn_71011735e8(shapes, &key, frame, 0);
}

static inline UnkVec3f Rotate(const UnkMatrix44Words& rotation, const UnkVec3f& v) {
    return TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&rotation), &v);
}

struct UnkStartSelf_i;

void fn_710110e554(UnkStartSelf_i* component) {
    UnkAIFollow* self = reinterpret_cast<UnkAIFollow*>(component);
    UnkAIFollowOwner* owner = self->owner;
    self->pos = PosOf(owner);
    if (UnkEntity* shapes = owner->shapes) {
        UnkAIFollowAnchor* anchor = self->anchor;
        base::global::UnkStringData* shape = anchor->shape;
        if (shape != &g_unk_7101d47108->empty) {
            UnkMatrix44 frame;
            for (u32 i = 0; i < 16; ++i)
                frame.m[i] = 0.0f;
            frame.m[15] = 1.0f;
            frame.m[10] = 1.0f;
            frame.m[5] = 1.0f;
            frame.m[0] = 1.0f;
            if (FindShape(shapes, shape, &frame)) {
                self->pos = TranslationOf(frame);
                UnkVec3f offset =
                    Rotate(CopyMatrixWithoutTranslation(reinterpret_cast<const UnkMatrix44Words*>(&frame)), self->anchor->offset);
                self->pos.x = offset.x + self->pos.x;
                self->pos.y = offset.y + self->pos.y;
                self->pos.z = offset.z + self->pos.z;
            }
        }
    }

    if (NearZero(self->last.x) && NearZero(self->last.y) && NearZero(self->last.z))
        self->last = self->pos;

    if (!self->vtable->vf_0x420(self))
        return;
    UnkAIFollowOwner* target = reinterpret_cast<UnkAIFollowOwner*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    if (!target)
        return;
    owner = self->owner;
    f32 dx = owner->x - self->pos.x;
    f32 dy = owner->y - self->pos.y;
    f32 dz = owner->z - self->pos.z;
    f32 t = self->time / 0.8f;
    t = t > 0.0f ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
    CVector3D to = PosOf(target);
    if (!(t >= 1.0f)) {
        f32 s = 1.0f - t;
        to.x = t * to.x + s * self->last.x;
        to.y = t * to.y + s * self->last.y;
        to.z = t * to.z + s * self->last.z;
    }
    self->pos = to;
    owner->x = dx + to.x;
    owner->y = dy + to.y;
    owner->z = dz + to.z;
    owner->moved = true;
}
