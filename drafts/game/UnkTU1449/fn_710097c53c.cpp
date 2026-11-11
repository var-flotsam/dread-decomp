#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" f32 sqrtf(f32);

using base::global::CStrId;

struct UnkBounds_b0197 {
    UnkVec3f min;
    UnkVec3f max;
    UnkBounds_b0197() {}
    UnkBounds_b0197(const UnkBounds_b0197& o) : min(o.min), max(o.max) {}
};

struct UnkBoundsSource_b0197 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBounds_b0197 vf_0x28(void* arg);
};

struct UnkBoundsObject_b0197 {
    u8 pad_0x0[0x158];
    UnkBoundsSource_b0197* source_0x158;
};

struct UnkPositioned_b0197 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
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
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual UnkVec3f vf_0x188();
};

struct UnkOwnerPart_b0197 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision_0x2a0;
};

struct UnkFollowTarget_b0197 {
    u8 pad_0x0[0x44];
    f32 x_0x44;
    u8 pad_0x48[0xc];
    f32 y_0x54;
    u8 pad_0x58[0xc];
    f32 z_0x64;
};

struct UnkFollower_b0197 {
    u8 pad_0x0[0x30];
    UnkOwnerPart_b0197* owner_0x30;
};

UnkPositioned_b0197* fn_710097e23c(UnkOwnerPart_b0197* owner);
UnkPositioned_b0197* fn_710097e318(UnkOwnerPart_b0197* owner);

struct UnkGameManagerIdTable_b0197 {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable_b0197() asm("fn_7100031894");
    ~UnkGameManagerIdTable_b0197() asm("fn_71000407f0");
};

inline const CStrId* ColliderId_b0197() {
    static UnkGameManagerIdTable_b0197 s_table asm("g_unk_7101d552d0");
    return reinterpret_cast<const CStrId*>(s_table.pad_0x0 + 0x1310);
}

UnkVec3f fn_710097c53c(UnkFollower_b0197* self, const UnkFollowTarget_b0197* target) {
    UnkOwnerPart_b0197* owner = self->owner_0x30;
    f32 reach;
    UnkCollisionContainer* container = owner->collision_0x2a0;
    UnkBoundsObject_b0197* object = nullptr;
    if (container)
        object = reinterpret_cast<UnkBoundsObject_b0197*>(fn_71008640a4(container, ColliderId_b0197()));
    if (object) {
        UnkBoundsSource_b0197* source = object->source_0x158;
        UnkBounds_b0197 bounds = source->vf_0x28(reinterpret_cast<u8*>(source) + 8);
        reach = (bounds.max.x - bounds.min.x) * 0.5f + 50.0f;
    } else {
        reach = 100.0f;
    }
    if (fn_710097e23c(owner))
        reach = 0.0f;
    UnkPositioned_b0197* actor = fn_710097e318(owner);
    f32 tx = target->x_0x44;
    f32 ty = target->y_0x54;
    f32 tz = target->z_0x64;
    f32 dx, dy, dz;
    {
        UnkVec3f p = actor->vf_0x188();
        dx = tx - p.x;
        dy = ty - p.y;
        dz = tz - p.z;
    }
    f32 len = sqrtf(dx * dx + dy * dy + dz * dz);
    f32 inv = 1.0f / (len + 0.0001f);
    f32 ux = dx * inv;
    f32 uy = dy * inv;
    f32 uz = dz * inv;
    f32 sx = reach * ux;
    f32 sy = reach * uy;
    f32 sz = reach * uz;
    UnkVec3f q = actor->vf_0x188();
    return UnkVec3f(sx + q.x, sy + q.y, sz + q.z);
}
