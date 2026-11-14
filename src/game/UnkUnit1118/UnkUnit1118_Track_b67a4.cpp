// Translation unit 1118 (tools/tu_map.py, no class name): follows a source. Without one it answers
// false. The previous 2D point (+0xcc) becomes the owner's position (+0x30, its matrix translation)
// while the mode at +0xac is 0x80000001, else the current point (+0xd4); the current point becomes the
// source's (its vf_0x18, through fn_710007b444) and +0xe4 the current point minus the owner's
// position. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// As src/game/navmesh/CNavMesh.cpp declares it.
struct UnkVec3;

// A 2D point copied member by member.
struct UnkFollowVec2_b67a4 {
    f32 x;
    f32 y;
    void Set(const UnkFollowVec2_b67a4& o) {
        x = o.x;
        y = o.y;
    }
};

struct UnkFollowSource_b67a4 {
    void** vtable;
    const UnkVec3* Position() {
        return reinterpret_cast<const UnkVec3* (*)(UnkFollowSource_b67a4*)>(vtable[0x18 / 8])(this);
    }
};

struct UnkFollowOwner_b67a4 {
    u8 pad_0x0[0x38];
    f32 m[12];  // 0x38, the transform's matrix
};

struct UnkFollow_b67a4 {
    u8 pad_0x0[0x30];
    UnkFollowOwner_b67a4* owner;  // 0x30
    u8 pad_0x38[0xac - 0x38];
    s32 mode;  // 0xac
    u8 pad_0xb0[0xcc - 0xb0];
    UnkFollowVec2_b67a4 prev;  // 0xcc
    UnkFollowVec2_b67a4 cur;  // 0xd4
    u8 pad_0xdc[0xe4 - 0xdc];
    UnkFollowVec2_b67a4 delta;  // 0xe4
};

// The owner's position (its matrix translation) projected to 2D; the caller's slots are passed in.
__attribute__((always_inline)) static inline void OwnerPos_b67a4(UnkFollow_b67a4* self, CVector3D* p,
                                                                 UnkFollowVec2_b67a4* out) {
    const f32* m = self->owner->m;
    u32 x, y, z;
    __builtin_memcpy(&x, &m[3], 4);
    __builtin_memcpy(&y, &m[7], 4);
    __builtin_memcpy(&z, &m[11], 4);
    __builtin_memcpy(&p->x, &x, 4);
    __builtin_memcpy(&p->y, &y, 4);
    __builtin_memcpy(&p->z, &z, 4);
    fn_710007b430(reinterpret_cast<CVector2D*>(out), p);
}

bool fn_71006fe35c(UnkFollow_b67a4* self, UnkFollowSource_b67a4* source) {
    if (!source)
        return false;
    {
        UnkFollowVec2_b67a4 v;
        CVector3D p;
        if (self->mode == static_cast<s32>(0x80000001))
            OwnerPos_b67a4(self, &p, &v);
        else
            v.Set(self->cur);
        self->prev.Set(v);
    }
    const UnkVec3* target = source->Position();
    asm("" : "+r"(self));
    fn_710007b444(&self->cur, target);
    UnkFollowVec2_b67a4 v;
    CVector3D p;
    OwnerPos_b67a4(self, &p, &v);
    f32 dx = self->cur.x - v.x;
    f32 dy = self->cur.y - v.y;
    self->delta.x = dx;
    self->delta.y = dy;
    return true;
}
