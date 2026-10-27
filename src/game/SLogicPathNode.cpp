// The logic path's unit (SLogicPathNode; translation unit 2657 of data/tu_map.csv): a path holds
// an array of 0x20-byte polymorphic nodes whose vf_0x18 gives the node's position.
// fn_7101059530 measures the path up to a node.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

extern "C" f32 sqrtf(f32);

struct UnkLogicPathNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* vf_0x18();  // the position
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};
static_assert(sizeof(UnkLogicPathNode) == 0x20, "UnkLogicPathNode layout");

struct UnkLogicPath {
    void** vtable;
    base::UnkVector<UnkLogicPathNode> nodes;
};

static inline f32 Distance(const CVector3D* a, const CVector3D* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

// The length of the path from its first node to node `index`, plus `fraction` of the segment
// after it (when there is one); 0 for a negative index.
// NON_MATCHING(minor): scheduling: in each distance the original loads b's z before a's (other
// registers for the z difference), and for index 0 it sets the sum before the counter.
f32 fn_7101059530(UnkLogicPath* self, s32 index, f32 fraction) {
    if (index < 0)
        return 0.0f;
    f32 length = 0.0f;
    s32 i = 1;
    for (; i <= index; ++i)
        length += Distance(self->nodes.data[i].vf_0x18(), self->nodes.data[i - 1].vf_0x18());
    if (i < (s32)self->nodes.count)
        length += Distance(self->nodes.data[(u32)i].vf_0x18(), self->nodes.data[(u32)index].vf_0x18()) * fraction;
    return length;
}
