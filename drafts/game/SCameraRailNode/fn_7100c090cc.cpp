// SCameraRailNode's unit (tools/tu_map.py): appends a copy of a camera-rail node (0x58 bytes: vtable, three
// words, a callback set) to an array of them, growing it first when it is full (each node moved over).
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

// The node as it is built in place: its vtable, the three words and its callback set.
struct UnkRailNodeRaw_lc090cc {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    UnkCallbackSet_7101a20950 set;

    UnkRailNodeRaw_lc090cc(const UnkRailNodeRaw_lc090cc& other)
        : vtable(g_unk_7101aa2888 + 0x10), field_0x8(other.field_0x8), field_0xc(other.field_0xc),
          field_0x10(other.field_0x10), set(other.set) {}
};
static_assert(sizeof(UnkRailNodeRaw_lc090cc) == 0x58, "UnkRailNodeRaw_lc090cc layout");

void fn_7100c090cc(base::global::CRntVector<UnkRailNode_b43a3>* self, const UnkRailNode_b43a3* value) {
    u32 n = self->count;
    u32 old_capacity = self->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkRailNode_b43a3* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkRailNode_b43a3*>(
            self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkRailNode_b43a3), 8, self->flags));
        if (UnkRailNode_b43a3* src = self->data) {
            UnkRailNode_b43a3* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                UnkRailNodeRaw_lc090cc* d = reinterpret_cast<UnkRailNodeRaw_lc090cc*>(dst);
                d->vtable = g_unk_7101aa2888 + 0x10;
                d->field_0x8 = src->field_0x8;
                d->field_0xc = src->field_0xc;
                d->field_0x10 = src->field_0x10;
                new (&dst->set) UnkHalf_cu(static_cast<UnkHalf_cu&&>(src->set));
                src->vf_0x8();
                ++src;
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        n = self->count;
        self->data = p;
        self->capacity = new_capacity;
    } else {
        p = self->data;
    }
    new (&p[n]) UnkRailNodeRaw_lc090cc(*reinterpret_cast<const UnkRailNodeRaw_lc090cc*>(value));
    ++self->count;
}
