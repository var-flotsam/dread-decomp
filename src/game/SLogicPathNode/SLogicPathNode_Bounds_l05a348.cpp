// SLogicPathNode unit: the bounds of the path's points (0x20-byte polymorphic elements, slot 3 answers a
// position), recomputed when the dirty flag is set; answers the box. Names and layouts are placeholders.
#include "game/UnkAabb.h"
#include "game/UnkVec3.h"
#include "types.h"

struct UnkPathPoint_l05a348 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3* vf_0x18();
    u8 pad_0x8[0x18];
};

struct UnkPathBounds {
    u8 pad_0x0[0x8];
    UnkPathPoint_l05a348* points;  // 0x8
    u32 count;             // 0x10
    u8 pad_0x14[0x1c];
    bool dirty;            // 0x30
    UnkAabb bounds;        // 0x34
};

static inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

UnkAabb* fn_710105a348(UnkPathBounds* self) {
    if (self->dirty) {
        self->bounds.min = UnkVec3{3.4028235e38f, 3.4028235e38f, 3.4028235e38f};
        self->bounds.max = UnkVec3{-3.4028235e38f, -3.4028235e38f, -3.4028235e38f};
        if (self->count) {
            UnkPathPoint_l05a348* p = self->points;
            do {
                const UnkVec3* v = p->vf_0x18();
                self->bounds.min = UnkVec3{Min(self->bounds.min.x, v->x), Min(self->bounds.min.y, v->y),
                                           Min(self->bounds.min.z, v->z)};
                self->bounds.max = UnkVec3{Max(self->bounds.max.x, v->x), Max(self->bounds.max.y, v->y),
                                           Max(self->bounds.max.z, v->z)};
                ++p;
            } while (p != self->points + self->count);
        }
        self->dirty = false;
    }
    return &self->bounds;
}
