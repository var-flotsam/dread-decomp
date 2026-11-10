// CBigkranXSpitAttack: the aim query fn_71003f2024 and what the attack keeps of its answer.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"

#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkRefCounted_ovb0267 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkHandleA_ovb0267 {
    UnkRefCounted_ovb0267* ptr;
    UnkHandleA_ovb0267() : ptr(nullptr) {}
    UnkHandleA_ovb0267& operator=(const UnkHandleA_ovb0267& o);  // fn_71002a5d1c
    ~UnkHandleA_ovb0267() {
        UnkRefCounted_ovb0267* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkHandleB_ovb0267 {
    UnkRefCounted_ovb0267* ptr;
    UnkHandleB_ovb0267() : ptr(nullptr) {}
    UnkHandleB_ovb0267& operator=(const UnkHandleB_ovb0267& o);  // fn_71002a5d1c
    ~UnkHandleB_ovb0267() {
        UnkRefCounted_ovb0267* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkAssignedSlot;
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);
inline UnkHandleA_ovb0267& UnkHandleA_ovb0267::operator=(const UnkHandleA_ovb0267& o) {
    fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&o));
    return *this;
}
inline UnkHandleB_ovb0267& UnkHandleB_ovb0267::operator=(const UnkHandleB_ovb0267& o) {
    fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&o));
    return *this;
}

struct UnkAimTrack_ovb0267 {
    UnkHandleA_ovb0267 a;
    UnkHandleB_ovb0267 b;
    s32 field_0x10;
    u32 field_0x14[12];
    UnkAimTrack_ovb0267() : field_0x10(-1) {
        for (int i = 0; i < 12; i++)
            field_0x14[i] = 0;
    }
    UnkAimTrack_ovb0267& operator=(const UnkAimTrack_ovb0267& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            field_0x10 = o.field_0x10;
            for (int i = 0; i < 12; i++)
                field_0x14[i] = o.field_0x14[i];
        }
        return *this;
    }
};

typedef u32 __attribute__((may_alias)) a_u32_ovb0267;
struct UnkSpitVec3_ovb0267 {
    f32 x, y, z;
    UnkSpitVec3_ovb0267() : x(0), y(0), z(0) {}
    UnkSpitVec3_ovb0267& operator=(const UnkSpitVec3_ovb0267& o) {
        ((a_u32_ovb0267*)this)[0] = ((const a_u32_ovb0267*)&o)[0];
        ((a_u32_ovb0267*)this)[1] = ((const a_u32_ovb0267*)&o)[1];
        ((a_u32_ovb0267*)this)[2] = ((const a_u32_ovb0267*)&o)[2];
        return *this;
    }
};

struct UnkSpitAim_ovb0267 {
    u8 pad_0x0[8];
    UnkActor* field_0x8;
    u8 pad_0x10[0x4c];
    UnkSpitVec3_ovb0267 field_0x5c;
    UnkSpitVec3_ovb0267 field_0x68;
    UnkSpitVec3_ovb0267 field_0x74;
    bool field_0x80;
    u8 pad_0x81[7];
    UnkAimTrack_ovb0267 field_0x88;
    f32 field_0xd0;
    f32 field_0xd4;
    f32 field_0xd8;
};

void fn_71003f2024(UnkSpitAim_ovb0267* self, bool a, bool b, const UnkSpitVec3_ovb0267* dir, bool* ok,
                   UnkAimTrack_ovb0267* track, UnkSpitVec3_ovb0267* p0, UnkSpitVec3_ovb0267* p1, f32* f0,
                   f32* f1, UnkSpitVec3_ovb0267* p2, f32* f2);

void fn_71003f18f8(UnkSpitAim_ovb0267* self, bool a, bool b, const UnkSpitVec3_ovb0267* dir) {
    bool ok = false;
    UnkAimTrack_ovb0267 track;
    UnkSpitVec3_ovb0267 p0;
    UnkSpitVec3_ovb0267 p1;
    f32 f0, f1;
    UnkSpitVec3_ovb0267 p2;
    f32 f2;
    fn_71003f2024(self, a, b, dir, &ok, &track, &p0, &p1, &f0, &f1, &p2, &f2);
    if (ok || !self->field_0x80) {
        self->field_0x80 = ok;
        self->field_0x88 = track;
        self->field_0x5c = p0;
        self->field_0x68 = p1;
        self->field_0xd4 = f0;
        self->field_0xd8 = f1;
        self->field_0x74 = p2;
        self->field_0xd0 = f2;
    }
}

bool fn_71003f1ba8(UnkSpitAim_ovb0267* self, bool b) {
    if (!self->field_0x8)
        return false;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    if (!target)
        return false;
    bool ok = false;
    UnkAimTrack_ovb0267 track;
    UnkSpitVec3_ovb0267 p0;
    UnkSpitVec3_ovb0267 p1;
    f32 f0 = -1.0f, f1 = -1.0f;
    UnkSpitVec3_ovb0267 p2;
    f32 f2;
    UnkActor* owner = self->field_0x8;
    f32 tx = target->field_0x44, ty = target->field_0x54, tz = target->field_0x64;
    f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
    f32 dx = tx - ox;
    f32 dy = ty - oy;
    f32 dz = tz - oz;
    const UnkMatrix44& mat = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    f32 m2 = mat.m[2], m6 = mat.m[6], m10 = mat.m[10];
    f32 side = dx * m2 + dy * m6 + dz * m10;
    UnkActor* o = self->field_0x8;
    UnkVec3f angle = reinterpret_cast<UnkXform*>(o->field_0x38)->GetAngle();
    UnkSpitVec3_ovb0267 dir;
    dir.x = angle.x;
    dir.y = angle.y;
    dir.z = angle.z;
    if (side < -0.001f)
        dir.y = -dir.y;
    fn_71003f2024(self, b, true, &dir, &ok, &track, &p0, &p1, &f0, &f1, &p2, &f2);
    return ok;
}
