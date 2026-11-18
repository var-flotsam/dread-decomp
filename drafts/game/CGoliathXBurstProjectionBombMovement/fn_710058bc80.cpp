// CGoliathXBurstProjectionBombMovement's unit: the bomb's collision with an actor. Unless the base movement
// handled it (fn_7100ac79d8) and the hit is not a new point (within 0.001 of one already listed at +0x4b8) on a
// receiver whose value at +0x54 is not below its value at +0x50, builds the damage request from the template
// at +0x348 (as CBombMovement's fn_7100aabdac does), hands it to the actor's receiver, remembers the point when
// the receiver does not answer 1 and runs the movement's slot 0x2a8. Names and layouts are placeholders.
#include "game/CGoliathXBurstProjectionBombMovement.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTU1370.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkBombHitSource_ovb0078 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkBombHitData_ovb0078 {
    UnkBombHitSource_ovb0078* source;
    u8 pad_0x8[0xc];
    UnkVec3f point;   // 0x14
    u8 pad_0x20[0xc];
    UnkVec3f normal;  // 0x2c
    u8 pad_0x38[4];
    u8 flags;         // 0x3c
};

struct UnkBombRequest_ovb0078 {
    u8 pad_0x0[0x60];
    void* field_0x60;
    s32 field_0x68;
    UnkVec3f origin;  // 0x6c
    UnkVec3f dir;     // 0x78
    UnkVec3f back;    // 0x84
    u32 field_0x90;
    u32 field_0x94;
    u8 pad_0x98[0x100 - 0x98];
};

struct UnkGoliathXBombReceiver_l58bc80 {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 field_0x50;
    f32 field_0x54;

    s32 vf_0x190(UnkActor* owner, UnkBombRequest_ovb0078* request, UnkBombHitData_ovb0078* hit) {
        return reinterpret_cast<s32 (*)(UnkGoliathXBombReceiver_l58bc80*, UnkActor*, UnkBombRequest_ovb0078*,
                                        UnkBombHitData_ovb0078*)>(vtable[0x190 / 8])(this, owner, request, hit);
    }
    s32 vf_0x188(UnkBombRequest_ovb0078* request) {
        return reinterpret_cast<s32 (*)(UnkGoliathXBombReceiver_l58bc80*, UnkBombRequest_ovb0078*)>(
            vtable[0x188 / 8])(this, request);
    }
};

struct UnkGoliathXBombTarget_l58bc80 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b8 - 0x68];
    UnkGoliathXBombReceiver_l58bc80* field_0x1b8;
};

struct UnkGoliathXBombPoint_l58bc80 {
    f32 x, y, z;
};

// The points already hit (pushed by fn_710058c768).
struct UnkGoliathXBombPoints_l58bc80 {
    UnkGoliathXBombPoint_l58bc80* data;
    u32 count;
};

struct UnkGoliathXBombMovement_l58bc80;
struct UnkGoliathXBombMovementVtbl_l58bc80 {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkGoliathXBombMovement_l58bc80* self, UnkGoliathXBombTarget_l58bc80* actor,
                     UnkBombHitData_ovb0078* hit);
};

struct UnkGoliathXBombMovement_l58bc80 {
    UnkGoliathXBombMovementVtbl_l58bc80* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x348 - 0x38];
    UnkBombRequest_ovb0078 request;  // 0x348
    u8 pad_0x448[0x4b8 - 0x448];
    UnkGoliathXBombPoints_l58bc80 points;  // 0x4b8
};

static inline bool Near(f32 a, f32 b) { return a + 0.001f >= b && a - 0.001f <= b; }

static inline bool Contains(const UnkGoliathXBombPoints_l58bc80* points, const UnkGoliathXBombPoint_l58bc80* p) {
    f32 x = p->x, y = p->y, z = p->z;
    const UnkGoliathXBombPoint_l58bc80* it = points->data;
    const UnkGoliathXBombPoint_l58bc80* end = it + points->count;
    for (; it != end; ++it) {
        if (Near(x, it->x) && Near(y, it->y) && Near(z, it->z))
            return true;
    }
    return false;
}

void fn_710058bc80(UnkGoliathXBombMovement_l58bc80* self, UnkGoliathXBombTarget_l58bc80* actor,
                   UnkGoliathXBombPoint_l58bc80* pos, void*, UnkBombHitData_ovb0078* hit) {
    bool handled = fn_7100ac79d8(self, actor, hit);
    UnkGoliathXBombReceiver_l58bc80* receiver = actor->field_0x1b8;
    bool new_hit = receiver && receiver->field_0x54 >= receiver->field_0x50 && !Contains(&self->points, pos);
    if (!new_hit && handled)
        return;
    UnkBombRequest_ovb0078 request;
    fn_71008c1090(reinterpret_cast<UnkGooplotRequest_b63a3*>(&request),
                  reinterpret_cast<const UnkGooplotRequest_b63a3*>(&self->request));
    f32 ax = actor->x, ay = actor->y, az = actor->z;
    UnkActor* owner = self->owner;
    f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
    f32 dx = ax - ox;
    f32 dy = ay - oy;
    f32 dz = az - oz;
    f32 inv = 1.0f / (__builtin_sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    UnkVec3f dir(dx * inv, dy * inv, dz * inv);
    UnkVec3f origin = hit->point;
    UnkVec3f back = (hit->flags & 0x10) ? hit->normal : UnkVec3f(-dir.x, -dir.y, -dir.z);
    request.field_0x68 = -1;
    request.origin = origin;
    request.dir = dir;
    request.back = back;
    request.field_0x90 = 0;
    request.field_0x94 = 0x38;
    request.field_0x60 = hit->source ? hit->source->field_0x10 : nullptr;
    if (receiver && receiver->vf_0x190(self->owner, &request, hit) != 2) {
        if (receiver->vf_0x188(&request) != 1)
            fn_710058c768(reinterpret_cast<UnkPushedVector*>(&self->points), reinterpret_cast<const UnkValues3*>(pos));
        self->vtbl->vf_0x2a8(self, actor, hit);
    }
    fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&request));
}
