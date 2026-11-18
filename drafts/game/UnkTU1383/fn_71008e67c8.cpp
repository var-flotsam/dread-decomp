// Unit 1383 (tools/tu_map.py): places the target's node. Takes the actor's rotation (its world matrix with the
// translation dropped and the scale divided out), turns its angles through fn_71008e62b8 (or, when the actor's
// model has the named frame, that frame's angles), and moves the target node to the offset rotated by them.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU251.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkMat4f;

// The rotation matrix for three angles, returned through x8.
UnkMatrix44 fn_7100078e54(f32 x, f32 y, f32 z);
// The rotation part of a transform, returned through x8.
UnkMatrix44 fn_7100076e30(UnkXform* x);
UnkVec3f fn_71008e62b8(const UnkVec3f* angles, u32 kind);
// The point the final matrix places.
__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_710164b1b0;

struct UnkPlaceInfo_l8e67c8 {
    u8 pad_0x0[0x4];
    u32 kind;  // 0x4
    u8 pad_0x8[0x8];
    UnkStringData* frame;  // 0x10: a string id
};

struct UnkPlaceActor_l8e67c8 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x228 - 0x94];
    UnkEntity* model;  // 0x228
};
static_assert(__builtin_offsetof(UnkPlaceActor_l8e67c8, model) == 0x228, "layout");

struct UnkPlaceNode_l8e67c8 {
    u8 pad_0x0[0x30];
    UnkVec3f pos;  // 0x30
    u8 pad_0x3c[0x1b8 - 0x3c];
    bool dirty;  // 0x1b8
};

struct UnkPlaceObject_l8e67c8 {
    u8 pad_0x0[0x78];
    UnkPlaceNode_l8e67c8* node;  // 0x78
};

struct UnkPlaceTarget_l8e67c8 {
    u8 pad_0x0[0x138];
    UnkPlaceObject_l8e67c8* object;  // 0x138
};

struct UnkPlaceOffset_l8e67c8 {
    f32 x, y, z;
};

struct UnkPlace_l8e67c8 {
    u8 pad_0x0[0x8];
    UnkPlaceInfo_l8e67c8* info;    // 0x8
    UnkPlaceActor_l8e67c8* actor;  // 0x10
    u8 pad_0x18[0x48 - 0x18];
    UnkPlaceOffset_l8e67c8 offset;  // 0x48
    u8 pad_0x54[0x120 - 0x54];
    UnkPlaceTarget_l8e67c8* target;  // 0x120
};

static inline f32 Inverse(f32 v) {
    return v == 0.0f ? 1.0f : 1.0f / v;
}

// The actor's rotation: its world matrix without the translation, with the scale divided out.
static inline UnkMatrix44 Unscaled(UnkXform& xform) {
    const UnkMatrix44& world = xform.GetMatrix();
    UnkMatrix44 rotation;
    UnkCopyMatrix(&rotation, world);
    rotation.m[3] = 0.0f;
    rotation.m[7] = 0.0f;
    rotation.m[11] = 0.0f;
    const UnkVec3f& scale = xform.scale;
    return fn_71000127c8(fn_7100079138(Inverse(scale.x), Inverse(scale.y), Inverse(scale.z)), rotation);
}

static inline UnkVec3f AngleOf(const UnkMatrix44& m, u32 kind) {
    UnkVec3f angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&m));
    return fn_71008e62b8(&angles, kind);
}

// The model's frame named by `key`.
static inline bool FrameOf(UnkEntity* model, u64 key, UnkMatrix44* out) {
    return fn_71011735e8(model, &key, out, 0);
}

void fn_71008e67c8(UnkPlace_l8e67c8* self) {
    if (!self->info)
        return;
    UnkPlaceActor_l8e67c8* actor = self->actor;
    if (!actor)
        return;
    UnkPlaceOffset_l8e67c8 offset = self->offset;
    u32 kind = self->info->kind;
    UnkVec3f angle = AngleOf(Unscaled(actor->xform), kind);
    UnkStringData* frame = self->info->frame;
    if (frame != &g_unk_7101d47108->empty) {
        if (UnkEntity* model = self->actor->model) {
            UnkMatrix44 m;
            if (FrameOf(model, frame->Key(), &m)) {
                UnkXform* xform = reinterpret_cast<UnkXform*>(&m);
                UnkVec3f s = fn_7100076cec(xform);
                if (s.x != 0.0f && s.y != 0.0f && s.z != 0.0f)
                    fn_71000774fc(xform);
                angle = AngleOf(fn_7100076e30(xform), kind);
            }
        }
    }
    UnkPlaceTarget_l8e67c8* target = self->target;
    UnkMatrix44 m = fn_71000127c8(fn_7100078e54(angle.x, angle.y, angle.z),
                                  fn_7100078e54(offset.x, offset.y, offset.z));
    UnkVec3f pos = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&m), &g_unk_710164b1b0);
    UnkPlaceObject_l8e67c8* object = target->object;
    UnkPlaceNode_l8e67c8* node = object->node;
    node->pos = pos;
    node->dirty = true;
    fn_71001bdc3c(object);
}
