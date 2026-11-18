// CPowerBombMovement's unit (tools/tu_map.py unit 1612): the power bomb touches an actor. Unless the hit
// was already handled and the point is not new, the bomb's damage request is sent to the actor's
// receiver with the owner moved to the hit point, and the point is remembered. Names and layouts are
// placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "game/UnkTU1370.h"
#include "game/UnkXformNode.h"
#include "types.h"

// Whether the hit was already handled (bombs that touch the player or something they ignore).
bool fn_7100ac79d8(void* self, void* actor, void* info);

struct UnkPbHitSource_laaeb6c {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkPbHitData_laaeb6c {
    UnkPbHitSource_laaeb6c* source;
    u8 pad_0x8[0xc];
    UnkVec3f point;  // 0x14
    u8 pad_0x20[0xc];
    UnkVec3f normal;  // 0x2c
    u8 pad_0x38[4];
    u8 flags;  // 0x3c
};

struct UnkPbRequest_laaeb6c {
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

struct UnkPbOwner_laaeb6c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x3b0 - 0x68];
    bool field_0x3b0;
};

struct UnkPbReceiver_laaeb6c {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 field_0x50;
    f32 field_0x54;

    s32 vf_0x188(UnkPbRequest_laaeb6c* request) {
        return reinterpret_cast<s32 (*)(UnkPbReceiver_laaeb6c*, UnkPbRequest_laaeb6c*)>(vtable[0x188 / 8])(
            this, request);
    }
    s32 vf_0x190(UnkPbOwner_laaeb6c* owner, UnkPbRequest_laaeb6c* request, UnkPbHitData_laaeb6c* hit) {
        return reinterpret_cast<s32 (*)(UnkPbReceiver_laaeb6c*, UnkPbOwner_laaeb6c*, UnkPbRequest_laaeb6c*,
                                        UnkPbHitData_laaeb6c*)>(vtable[0x190 / 8])(this, owner, request, hit);
    }
};

struct UnkPbTargetActor_laaeb6c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0x1b8 - 0x58];
    UnkPbReceiver_laaeb6c* field_0x1b8;
};

struct UnkPbCollideMovement_laaeb6c;

struct UnkPbCollideMovementVtbl_laaeb6c {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkPbCollideMovement_laaeb6c* self, UnkPbTargetActor_laaeb6c* actor,
                     UnkPbHitData_laaeb6c* hit);
};

struct UnkPbCollideMovement_laaeb6c {
    UnkPbCollideMovementVtbl_laaeb6c* vtbl;
    u8 pad_0x8[0x28];
    UnkPbOwner_laaeb6c* owner;  // 0x30
    u8 pad_0x38[0x348 - 0x38];
    UnkPbRequest_laaeb6c request;  // 0x348
    u8 pad_0x448[0x4b8 - 0x448];
    base::global::CRntVector<UnkVec3f> points;  // 0x4b8, where the bomb has already hit
};

static inline bool IsNear(f32 a, f32 b) {
    f32 low = a - 0.001f;
    f32 high = a + 0.001f;
    return high >= b && low <= b;
}

static inline bool HasPoint(const base::global::CRntVector<UnkVec3f>& points, const UnkVec3f& pos) {
    for (const UnkVec3f& p : points) {
        if (IsNear(pos.x, p.x) && IsNear(pos.y, p.y) && IsNear(pos.z, p.z))
            return true;
    }
    return false;
}

void fn_7100aaeb6c(UnkPbCollideMovement_laaeb6c* self, UnkPbTargetActor_laaeb6c* actor, const UnkVec3f* pos,
                   void*, UnkPbHitData_laaeb6c* hit) {
    bool handled = fn_7100ac79d8(self, actor, hit);
    UnkPbReceiver_laaeb6c* receiver = actor->field_0x1b8;
    bool fresh = receiver && receiver->field_0x54 >= receiver->field_0x50 && !HasPoint(self->points, *pos);
    if (!fresh && handled)
        return;
    UnkPbRequest_laaeb6c request;
    fn_71008c1090(reinterpret_cast<UnkGooplotRequest_b63a3*>(&request),
                  reinterpret_cast<const UnkGooplotRequest_b63a3*>(&self->request));
    f32 ax = actor->x, ay = actor->y;
    UnkPbOwner_laaeb6c* from = self->owner;
    f32 ox = from->x, oy = from->y;
    f32 dx = ax - ox;
    f32 dy = ay - oy;
    f32 dz = 0.0f;
    f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
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
        // The receiver sees the owner at the hit point.
        UnkPbOwner_laaeb6c* owner = self->owner;
        UnkVec3f at = *pos;
        owner->field_0x3b0 = true;
        f32 x = owner->x;
        f32 y = owner->y;
        owner->x = at.x;
        owner->y = at.y;
        f32 z = owner->z;
        owner->z = at.z;
        s32 result = receiver->vf_0x188(&request);
        owner->x = x;
        owner->y = y;
        owner->z = z;
        owner->field_0x3b0 = true;
        self->points.PushBackCopy(*pos);
        if (result == 1)
            self->vtbl->vf_0x2a8(self, actor, hit);
    }
    fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&request));
}
