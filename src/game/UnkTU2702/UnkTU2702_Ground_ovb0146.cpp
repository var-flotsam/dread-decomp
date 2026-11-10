// Unit 2702 (hint CAIComponent): an AI component probes the ground below its owner. When the probe finds
// nothing the hit is filled in by hand (the owner's own shape, a point one extent below it along its
// up axis) before it is handed to the movement. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct CVector3D;
struct UnkCharacterMovement;
struct UnkCollisionHit;
struct UnkShape;
struct UnkFlagged;
template <u32 Offset>
struct UnkWeakRefAt;

bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);
UnkShape* fn_7100976588(UnkFlagged* flagged);
f32 fn_7100977b20(void*);
void fn_710092ef94(UnkWeakRefAt<0>* self, UnkWeakTarget* other);

struct UnkGroundRef_ovb0146 {
    UnkRefObject* ptr;

    UnkGroundRef_ovb0146() : ptr(nullptr) {}
    ~UnkGroundRef_ovb0146() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkGroundHit_ovb0146 {
    UnkGroundRef_ovb0146 object;
    UnkGroundRef_ovb0146 field_0x8;
    s32 id;
    f32 values[12];

    UnkGroundHit_ovb0146() : id(-1), values() {}
};

struct UnkGroundOwner_ovb0146 {
    u8 pad_0x0[0x38];
    void* xform;  // 0x38
    u8 pad_0x40[4];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b0];
    void* movement;  // 0x218
    u8 pad_0x220[0x20];
    void** ai;  // 0x240
};

struct UnkGroundMovement_ovb0146 {
    void** vtable;
    u8 pad_0x8[0x3c8];
    u32 mask;  // 0x3d0

    UnkCharacterMovement* vf_0x2a8() {
        return reinterpret_cast<UnkCharacterMovement* (*)(UnkGroundMovement_ovb0146*)>(vtable[0x2a8 / 8])(this);
    }
    void vf_0x218(UnkGroundHit_ovb0146* hit) {
        reinterpret_cast<void (*)(UnkGroundMovement_ovb0146*, UnkGroundHit_ovb0146*)>(vtable[0x218 / 8])(this, hit);
    }
};

struct UnkGroundAI_ovb0146 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGroundOwner_ovb0146* owner;  // 0x30
};

void fn_7101110030(UnkGroundAI_ovb0146* self) {
    UnkGroundMovement_ovb0146* movement = static_cast<UnkGroundMovement_ovb0146*>(self->owner->movement);
    if (!movement)
        return;
    UnkGroundHit_ovb0146 hit;
    UnkCharacterMovement* probe = movement->vf_0x2a8();
    UnkGroundOwner_ovb0146* owner = self->owner;
    f32 px = owner->x, py = owner->y, pz = owner->z;
    u32 mask = movement->mask;
    UnkVec3f pos(px, py, pz);
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&owner->xform));
    UnkVec3f dir(-m.m[1], -m.m[5], -m.m[9]);
    if (!fn_7100977e50(probe, 1.0f, 50.0f, 3.4028235e38f, 0.0f, mask, reinterpret_cast<CVector3D*>(&pos), nullptr,
                       reinterpret_cast<UnkCollisionHit*>(&hit), reinterpret_cast<CVector3D*>(&dir))) {
        UnkShape* shape = fn_7100976588(reinterpret_cast<UnkFlagged*>(self->owner->movement));
        fn_710092ef94(reinterpret_cast<UnkWeakRefAt<0>*>(&hit), reinterpret_cast<UnkWeakTarget*>(shape));
        UnkGroundOwner_ovb0146* o = self->owner;
        f32 x = o->x, y = o->y, z = o->z;
        const UnkMatrix44& m1 = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&o->xform));
        f32 ux = m1.m[1], uy = m1.m[5], uz = m1.m[9];
        f32 extent = fn_7100977b20(self->owner->movement);
        x -= ux * extent;
        y -= uy * extent;
        z -= uz * extent;
        const UnkMatrix44& m2 = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&self->owner->xform));
        u32 nx = *reinterpret_cast<const u32*>(&m2.m[1]);
        u32 ny = *reinterpret_cast<const u32*>(&m2.m[5]);
        u32 nz = *reinterpret_cast<const u32*>(&m2.m[9]);
        const UnkMatrix44& m3 = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&self->owner->xform));
        f32 dx = -m3.m[1], dy = -m3.m[5], dz = -m3.m[9];
        hit.id = -1;
        hit.values[0] = x;
        hit.values[1] = y;
        hit.values[2] = z;
        hit.values[3] = dx;
        hit.values[4] = dy;
        hit.values[5] = dz;
        __builtin_memcpy(&hit.values[6], &nx, 4);
        __builtin_memcpy(&hit.values[7], &ny, 4);
        __builtin_memcpy(&hit.values[8], &nz, 4);
        __builtin_memset(&hit.values[9], 0, 4);
        u32 flags = 0x38;
        __builtin_memcpy(&hit.values[10], &flags, 4);
    }
    movement->vf_0x218(&hit);
    reinterpret_cast<void (*)(void*)>((*reinterpret_cast<void***>(self->owner->ai))[0x3d8 / 8])(self->owner->ai);
}
