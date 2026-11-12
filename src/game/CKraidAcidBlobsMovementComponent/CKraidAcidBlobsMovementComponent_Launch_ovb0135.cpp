// CKraidAcidBlobsMovementComponent's unit (tools/tu_map.py): the blob's launch. After the base's
// (fn_7100ac3880) the blob registers with the game manager under its id (g_unk_71726f4060); when the
// definition (+0x5d0) asks for it (+0x11a), the target's position (fn_7100d7be08) scaled by +0x11c is
// added to the velocity (+0x4b8); the owner's trail part (fn_7101152b90) is placed at the owner and
// turned along the direction at +0x10c. Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "game/CProjectileMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSelf;
struct UnkWeakTarget;
extern __attribute__((visibility("hidden"))) const u64 g_unk_71726f4060;

struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkComponent;
struct UnkObject;
struct UnkPart;
void fn_7100145f28(UnkSwitchObject* self, const UnkVec3f* position);
void fn_7100145f3c(UnkSwitchObject* self, const UnkVec3f* angles);
extern "C" f32 atan2f(f32 y, f32 x);

struct UnkAcidBlobPart_ovb0135 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
};

struct UnkAcidBlobOwner_ovb0135 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38: the position is its translation
    u8 pad_0x94[0x1ec];
    UnkObject* model;  // 0x280
    u8 pad_0x288[0x138];
    bool dirty;  // 0x3c0

    UnkXform* Xform() { return &xform; }
};
static_assert(__builtin_offsetof(UnkAcidBlobOwner_ovb0135, dirty) == 0x3c0, "layout");

struct UnkAcidBlobDef_ovb0135 {
    u8 pad_0x0[0x11a];
    bool follow;  // 0x11a
    u8 pad_0x11b;
    f32 follow_scale;  // 0x11c
    u8 pad_0x120[0x14];
    f32 spin;  // 0x134
};

struct UnkAcidBlobTarget_ovb0135 {
    u8 pad_0x0[0x10];
    void* actor;  // 0x10
};


struct UnkAcidBlob_ovb0135 {
    u8 pad_0x0[0x30];
    UnkAcidBlobOwner_ovb0135* owner;  // 0x30
    u8 pad_0x38[0xd4];
    f32 dir_x;  // 0x10c
    f32 dir_y;  // 0x110
    f32 dir_z;  // 0x114
    u8 pad_0x118[0x118];
    UnkAcidBlobTarget_ovb0135* target;  // 0x230
    u8 pad_0x238[0x280];
    f32 vel_x;  // 0x4b8
    f32 vel_y;  // 0x4bc
    f32 vel_z;  // 0x4c0
    u8 pad_0x4c4[0xad];
    bool landed;  // 0x571
    u8 pad_0x572[0x5e];
    UnkAcidBlobDef_ovb0135* def;  // 0x5d0
    u8 pad_0x5d8[0x10];
    s32 size_step;  // 0x5e8
};
static_assert(__builtin_offsetof(UnkAcidBlob_ovb0135, def) == 0x5d0, "layout");

// Places the owner's trail part (fn_7101152b90) at the owner, turned along the direction at +0x10c.
static inline void PlaceTrail(UnkAcidBlob_ovb0135* self) {
    UnkObject* model = self->owner->model;
    auto* part = reinterpret_cast<UnkAcidBlobPart_ovb0135*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(model),
                                                                          0x1ce2163e2d5b53c6ull));
    if (!part || !fn_710114e680(model, part->kind))
        return;
    UnkVec3f angles;
    angles.x = 0.0f;
    angles.y = 1.57f;
    f32 dx = self->dir_x, dy = self->dir_y, dz = self->dir_z;
    f32 inv = 1.0f / (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    f32 nx = dx * inv;
    f32 ny = dy * inv;
    angles.z = atan2f(ny, nx);
    UnkAcidBlobOwner_ovb0135* owner = self->owner;
    UnkVec3f at(owner->xform.m[0][3], owner->xform.m[1][3], owner->xform.m[2][3]);
    UnkVec3f pos = at;
    fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(part), &pos);
    fn_7100145f3c(reinterpret_cast<UnkSwitchObject*>(part), &angles);
}

void fn_71006047d0(UnkAcidBlob_ovb0135* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    fn_7100de9540(g_unk_7101d47310, &g_unk_71726f4060, reinterpret_cast<UnkWeakTarget*>(self));
    UnkAcidBlobTarget_ovb0135* target = self->target;
    if (target && target->actor && self->def->follow) {
        UnkKraidCreatureVec t = fn_7100d7be08(target->actor);
        f32 k = self->def->follow_scale;
        f32 x = k * t.x, y = k * t.y, z = k * t.z;
        self->vel_x = x + self->vel_x;
        self->vel_y = y + self->vel_y;
        self->vel_z = z + self->vel_z;
    }
    PlaceTrail(self);
}
