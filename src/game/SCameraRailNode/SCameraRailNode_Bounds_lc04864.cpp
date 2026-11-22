// Unit SCameraRailNode: the bounds of the node's elements (0x50-byte polymorphic elements, slot 7 answers a
// box), recomputed when the dirty flag is set; answers the box. Names and layouts are placeholders.
#include "game/UnkAabb.h"
#include "game/UnkVec3.h"
#include "types.h"

struct UnkRailElement_c04864 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual const UnkAabb* vf_0x38();
    u8 pad_0x8[0x48];
};

struct UnkRailBounds_c04864 {
    u8 pad_0x0[0x10];
    UnkRailElement_c04864* elements;  // 0x10
    u32 count;                        // 0x18
    u8 pad_0x1c[0x28];
    bool dirty;                       // 0x44
    UnkAabb bounds;                   // 0x48
};

static inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

UnkAabb* fn_7100c04864(UnkRailBounds_c04864* self) {
    if (self->dirty) {
        self->bounds.min = UnkVec3{3.4028235e38f, 3.4028235e38f, 3.4028235e38f};
        self->bounds.max = UnkVec3{-3.4028235e38f, -3.4028235e38f, -3.4028235e38f};
        if (self->count) {
            UnkRailElement_c04864* p = self->elements;
            do {
                const UnkAabb* v = p->vf_0x38();
                self->bounds.min = UnkVec3{Min(self->bounds.min.x, v->min.x), Min(self->bounds.min.y, v->min.y),
                                           Min(self->bounds.min.z, v->min.z)};
                self->bounds.max = UnkVec3{Max(self->bounds.max.x, v->max.x), Max(self->bounds.max.y, v->max.y),
                                           Max(self->bounds.max.z, v->max.z)};
                ++p;
            } while (p != self->elements + self->count);
        }
        self->dirty = false;
    }
    return &self->bounds;
}
