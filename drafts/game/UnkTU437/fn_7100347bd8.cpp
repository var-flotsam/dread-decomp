// Unit 437 (CAimComponent): builds the aim frame for an angle: the aim direction (the facing side turned by
// the angle), the origin (the aim node's position from the animation, the owner's position without one), and
// the corners and ends of the aim box from the side vector, width and lengths. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkEntity;
UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);
f32 fn_7101177e24(UnkAnimationComponent* self, u64 key);
void fn_7101179e68(UnkAnimationComponent* self, void* entry, u64 key, UnkMatrix44* out, u32 flags, f32 frame);

struct UnkAimFrameOwner_l347bd8 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x18c];
    UnkAnimationComponent* animation;  // 0x220
    UnkEntity* entity;                 // 0x228
};
static_assert(__builtin_offsetof(UnkAimFrameOwner_l347bd8, animation) == 0x220, "layout");

struct UnkAimFrameTunable_l347bd8 {
    u8 pad_0x0[0x5c];
    f32 length;  // 0x5c
};

struct UnkAimFrameSelf_l347bd8 {
    u8 pad_0x0[0x30];
    UnkAimFrameOwner_l347bd8* owner;  // 0x30
    u8 pad_0x38[0x278];
    UnkAimFrameTunable_l347bd8* tunable;  // 0x2b0
    u8 pad_0x2b8[0x2bc];
    f32 width;   // 0x574
    f32 length;  // 0x578
};
static_assert(__builtin_offsetof(UnkAimFrameSelf_l347bd8, tunable) == 0x2b0, "layout");
static_assert(__builtin_offsetof(UnkAimFrameSelf_l347bd8, width) == 0x574, "layout");

struct UnkAimFrame_l347bd8 {
    u8 pad_0x0[0x50];
    UnkVec3f origin;      // 0x50
    UnkVec3f left;        // 0x5c
    UnkVec3f right;       // 0x68
    UnkVec3f leftNear;    // 0x74
    UnkVec3f rightNear;   // 0x80
    UnkVec3f leftFar;     // 0x8c
    UnkVec3f rightFar;    // 0x98
};

static inline UnkVec3f Cross(const UnkVec3f& a, const UnkVec3f& b) {
    UnkVec3f r;
    r.x = b.z * a.y - b.y * a.z;
    r.y = b.x * a.z - b.z * a.x;
    r.z = b.y * a.x - b.x * a.y;
    return r;
}

static inline u64 AimNodeKey() {
    const base::global::CStrId& id =
        *reinterpret_cast<const base::global::CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + 0xd58);
    return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
}

static const u64 kAimKey = 0xf4061ff25a2163d6;

void fn_7100347bd8(UnkAimFrameSelf_l347bd8* self, f32 angle, UnkAimFrame_l347bd8* out) {
    f32 facing = self->owner->xform.GetAngle().y;
    UnkVec3f dir = facing < 0.0f ? RotateVec3AboutZ(UnkVec3f(-1.0f, 0.0f, 0.0f), -angle)
                                 : RotateVec3AboutZ(UnkVec3f(1.0f, 0.0f, 0.0f), angle);
    UnkVec3f side = Cross(UnkVec3f(0.0f, 0.0f, -1.0f), dir);

    UnkAimFrameOwner_l347bd8* owner = self->owner;
    UnkAnimationComponent* animation = owner->animation;
    out->origin = owner->xform.GetTranslation();
    if (animation) {
        UnkMatrix44 m = {};
        m.m[15] = 1.0f;
        m.m[10] = 1.0f;
        m.m[5] = 1.0f;
        m.m[0] = 1.0f;
        UnkTrackAction* action = fn_710070d500(fn_71011771e0(animation, kAimKey));
        if (action &&
            fn_710009d50c(reinterpret_cast<const base::reflection::CType* (*)(UnkTrackAction*)>(action->vtable[3])(
                              action),
                          fn_7100c9b4f8())) {
            f32 frame = fn_7101177e24(animation, kAimKey) * angle / 6.2831855f;
            void* entry = fn_7101177d2c(animation, kAimKey);
            fn_7101179e68(animation, entry, AimNodeKey(), &m, 0, frame);
            out->origin = UnkVec3f(m.m[3], m.m[7], m.m[11]);
        } else if (UnkEntity* entity = self->owner->entity) {
            {
                u64 key = AimNodeKey();
                fn_71011735e8(entity, &key, &m, 0);
            }
            out->origin = UnkVec3f(m.m[3], m.m[7], m.m[11]);
        }
    }

    const UnkVec3f& o = out->origin;
    out->left = UnkVec3f(o.x - side.x * self->width, o.y - side.y * self->width, o.z - side.z * self->width);
    out->right = UnkVec3f(o.x + side.x * self->width, o.y + side.y * self->width, o.z + side.z * self->width);
    out->leftNear = UnkVec3f(self->length * dir.x + out->left.x, self->length * dir.y + out->left.y,
                             self->length * dir.z + out->left.z);
    out->rightNear = UnkVec3f(out->right.x + dir.x * self->length, out->right.y + dir.y * self->length,
                              out->right.z + dir.z * self->length);
    f32 far = self->tunable->length - self->length;
    out->leftFar = UnkVec3f(out->leftNear.x + dir.x * far, out->leftNear.y + dir.y * far,
                            out->leftNear.z + dir.z * far);
    far = self->tunable->length - self->length;
    out->rightFar = UnkVec3f(out->rightNear.x + dir.x * far, out->rightNear.y + dir.y * far,
                             out->rightNear.z + dir.z * far);
}
