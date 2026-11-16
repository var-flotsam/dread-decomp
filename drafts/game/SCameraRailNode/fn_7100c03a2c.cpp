// SCameraRailNode's unit (tools/tu_map.py): the camera-rail node (0x58 bytes) at an index of an array of them,
// growing the array up to it with default nodes when it is past the end: CRntVector::GrowAt written out.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/SCameraRailNode.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkRailNode_b43a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();  // destroys
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    UnkHalf_cu set;  // 0x18
};
static_assert(sizeof(UnkRailNode_b43a3) == 0x58, "UnkRailNode_b43a3 layout");

// A node as it is moved or built in place.
struct UnkRailNodeRaw_lc03a2c {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    UnkHalf_cu set;
};

struct UnkRailNodeNew_lc03a2c {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    UnkCallbackSet_7101a20950 set;

    UnkRailNodeNew_lc03a2c() : vtable(g_unk_7101aa2888 + 0x10), field_0x8(0), field_0xc(0), field_0x10(0) {}
};
static_assert(sizeof(UnkRailNodeNew_lc03a2c) == 0x58, "UnkRailNodeNew_lc03a2c layout");

using UnkRailNodes_lc03a2c = base::global::CRntVector<UnkRailNode_b43a3>;

// Allocates n nodes and moves the nodes over, freeing the old array.
static inline void Reallocate(UnkRailNodes_lc03a2c* v, u32 n) {
    UnkRailNode_b43a3* p =
        static_cast<UnkRailNode_b43a3*>(v->allocator->vf_0x40((u64)n * sizeof(UnkRailNode_b43a3), 8, v->flags));
    if (UnkRailNode_b43a3* src = v->data) {
        UnkRailNode_b43a3* dst = p;
        for (u32 i = 0; i < v->count; ++i) {
            UnkRailNodeRaw_lc03a2c* d = reinterpret_cast<UnkRailNodeRaw_lc03a2c*>(dst);
            d->vtable = g_unk_7101aa2888 + 0x10;
            d->field_0x8 = src->field_0x8;
            d->field_0xc = src->field_0xc;
            d->field_0x10 = src->field_0x10;
            new (&d->set) UnkHalf_cu(static_cast<UnkHalf_cu&&>(src->set));
            src->vf_0x8();
            ++src;
            ++dst;
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = p;
    v->capacity = n;
}

void* fn_7100c03a2c(void* target, u32 index) {
    auto* nodes = static_cast<UnkRailNodes_lc03a2c*>(target);
    if (nodes->field_0x14 && nodes->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 nodes.
        u32 cap = nodes->capacity;
        if (cap <= index && nodes->step != 0) {
            u32 grown = (index / nodes->step + 1) * nodes->step;
            if (cap < grown) {
                Reallocate(nodes, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(nodes, n);
        for (u32 i = nodes->count; i < n; ++i)
            new (&nodes->data[i]) UnkRailNodeNew_lc03a2c();
        nodes->count = n;
    }
    return &nodes->data[index];
}
