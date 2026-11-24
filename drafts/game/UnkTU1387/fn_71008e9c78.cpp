// Unit 1387 (tools/tu_map.py, no name hint): points the light reached through the owner's +0x138 and +0x78
// along the object's orientation. The rotation is taken from the object's transform (scale divided out), or,
// when the definition names a node the object's model knows, from that node's unscaled matrix; each is turned
// into a light value for the definition's mode (fn_71008e62b8), combined with the light's own angles (+0x48)
// and applied to the forward axis. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU251.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// Returned in memory: the vector type has a user-written copy constructor.
struct UnkVec3_b42 {
    f32 x, y, z;
    UnkVec3_b42(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3_b42(const UnkVec3_b42& o) : x(o.x), y(o.y), z(o.z) {}
};
UnkVec3_b42 fn_71008e62b8(const UnkVec3_b42* value, s32 mode);

UnkMatrix44 fn_7100076e30(const UnkMatrix44& src);
UnkMatrix44 fn_7100078e54(f32 a, f32 b, f32 c);

extern const UnkVec3f g_unk_710164b1cc __attribute__((visibility("hidden")));  // the forward axis

struct UnkLight_l8e9c78 {
    u8 pad_0x0[0x30];
    UnkVec3f direction;  // 0x30
    u8 pad_0x3c[0x17c];
    bool dirty;  // 0x1b8
};
static_assert(__builtin_offsetof(UnkLight_l8e9c78, dirty) == 0x1b8, "layout");

struct UnkLightHolder_l8e9c78 {
    u8 pad_0x0[0x78];
    UnkLight_l8e9c78* light;  // 0x78
};

struct UnkLightOwner_l8e9c78 {
    u8 pad_0x0[0x138];
    UnkLightHolder_l8e9c78* holder;  // 0x138
};

struct UnkNodeName_l8e9c78 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkLightDef_l8e9c78 {
    u8 pad_0x0[0x4];
    s32 mode;  // 0x4
    u8 pad_0x8[0x8];
    UnkNodeName_l8e9c78* node;  // 0x10: a string id's data
};

struct UnkLightObject_l8e9c78 {
    u8 pad_0x0[0x38];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x194];
    UnkEntity* model;  // 0x228
};
static_assert(__builtin_offsetof(UnkLightObject_l8e9c78, model) == 0x228, "layout");

struct UnkLightAim_l8e9c78 {
    u8 pad_0x0[0x8];
    UnkLightDef_l8e9c78* def;          // 0x8
    UnkLightObject_l8e9c78* object;    // 0x10
    u8 pad_0x18[0x30];
    f32 angle[3];  // 0x48
    u8 pad_0x54[0xcc];
    UnkLightOwner_l8e9c78* owner;  // 0x120
};
static_assert(__builtin_offsetof(UnkLightAim_l8e9c78, owner) == 0x120, "layout");

// The matrix with its translation cleared.
static inline UnkMatrix44 WithoutTranslation(const UnkMatrix44& src) {
    UnkMatrix44 r;
    UnkCopyMatrix(&r, src);
    r.m[3] = 0.0f;
    r.m[7] = 0.0f;
    r.m[11] = 0.0f;
    return r;
}

static inline f32 Inverse(f32 v) {
    return v == 0.0f ? 1.0f : 1.0f / v;
}

// The transform's rotation with its scale divided out.
static inline UnkMatrix44 Unscaled(UnkXform& transform) {
    UnkMatrix44 rotation = WithoutTranslation(fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&transform)));
    const UnkVec3f& scale = transform.scale;
    return fn_71000127c8(fn_7100079138(Inverse(scale.x), Inverse(scale.y), Inverse(scale.z)), rotation);
}

// The light value for a rotation matrix's angles.
static inline UnkVec3_b42 LightValue(const UnkMatrix44& m, s32 mode) {
    UnkVec3f angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&m));
    return fn_71008e62b8(reinterpret_cast<const UnkVec3_b42*>(&angles), mode);
}

// Whether no scale factor is zero.
static inline bool HasScale(const UnkVec3f& s) {
    return s.x != 0.0f && s.y != 0.0f && s.z != 0.0f;
}

// The matrix of the model's node named by `key`.
static inline bool NodeMatrix(UnkEntity* model, u64 key, UnkMatrix44* out) {
    return fn_71011735e8(model, &key, out, 0);
}

void fn_71008e9c78(UnkLightAim_l8e9c78* self) {
    if (!self->def)
        return;
    UnkLightObject_l8e9c78* object = self->object;
    if (!object)
        return;
    f32 ax = self->angle[0];
    f32 ay = self->angle[1];
    f32 az = self->angle[2];
    s32 mode = self->def->mode;
    UnkVec3_b42 value = LightValue(Unscaled(object->transform), mode);
    UnkNodeName_l8e9c78* node = self->def->node;
    if (node != reinterpret_cast<UnkNodeName_l8e9c78*>(&g_unk_7101d47108->empty)) {
        if (UnkEntity* model = self->object->model) {
            UnkMatrix44 matrix;
            if (NodeMatrix(model, node->hash, &matrix)) {
                if (HasScale(fn_7100076cec(reinterpret_cast<UnkXform*>(&matrix))))
                    fn_71000774fc(reinterpret_cast<UnkXform*>(&matrix));
                value = LightValue(fn_7100076e30(matrix), mode);
            }
        }
    }
    UnkLightOwner_l8e9c78* owner = self->owner;
    UnkMatrix44 orientation = fn_71000127c8(fn_7100078e54(value.x, value.y, value.z), fn_7100078e54(ax, ay, az));
    UnkVec3f direction = TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&orientation), &g_unk_710164b1cc);
    UnkLightHolder_l8e9c78* holder = owner->holder;
    holder->light->direction = direction;
    holder->light->dirty = true;
    fn_71001bdc3c(holder);
}
