// SNavMeshPath unit: recomputes the bounds of the path's elements (0x120-byte polymorphic, slot 3 answers a
// position) when the dirty flag is set; answers the box. Names and layouts are placeholders.
#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

struct UnkAabb {
    UnkVec3 min;
    UnkVec3 max;
};

struct UnkNavElement_l2ae1b0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3* vf_0x18();
    u8 pad_0x8[0x118];
};

#pragma pack(push, 4)
struct UnkNavMeshPath_l2ae1b0 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    UnkNavElement_l2ae1b0 elements[1];  // 0xc
    u8 pad_0x12c[0x11820 - 0x120 + 0x120];
    bool dirty;  // 0x1194c
    UnkAabb bounds;  // 0x11950
};
#pragma pack(pop)

static_assert(__builtin_offsetof(UnkNavMeshPath_l2ae1b0, dirty) == 0x1194c);
static_assert(__builtin_offsetof(UnkNavMeshPath_l2ae1b0, bounds) == 0x11950);

static inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

UnkAabb* fn_71002ae1b0(UnkNavMeshPath_l2ae1b0* self) {
    if (self->dirty) {
        self->bounds.min = UnkVec3{3.4028235e38f, 3.4028235e38f, 3.4028235e38f};
        self->bounds.max = UnkVec3{-3.4028235e38f, -3.4028235e38f, -3.4028235e38f};
        UnkNavElement_l2ae1b0* first = self->elements;
        for (UnkNavElement_l2ae1b0* p = first; p != first + self->count; ++p) {
            const UnkVec3* v = p->vf_0x18();
            self->bounds.min = UnkVec3{Min(self->bounds.min.x, v->x), Min(self->bounds.min.y, v->y),
                                       Min(self->bounds.min.z, v->z)};
            self->bounds.max = UnkVec3{Max(self->bounds.max.x, v->x), Max(self->bounds.max.y, v->y),
                                       Max(self->bounds.max.z, v->z)};
        }
        self->dirty = false;
    }
    return &self->bounds;
}
