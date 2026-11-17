// SCameraRailNode's unit (tools/tu_map.py): moves a camera-rail node (0x58 bytes: vtable, three words, a
// callback set) into an array of them at an index, shifting the later nodes up one place (growing the
// array first when it is full): CRntVector::Insert written out. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/SCameraRailNode.h"
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

// A node as its move constructor builds it: the vtable, the three words and the callback set moved over.
struct UnkRailNodeRaw_lc09460 {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    UnkHalf_cu set;
};

using UnkRailNodes_lc09460 = base::global::CRntVector<UnkRailNode_b43a3>;

// Builds `dst` by moving `src` into it.
static inline void MoveNode(UnkRailNode_b43a3* dst, UnkRailNode_b43a3* src) {
    UnkRailNodeRaw_lc09460* d = reinterpret_cast<UnkRailNodeRaw_lc09460*>(dst);
    d->vtable = g_unk_7101aa2888 + 0x10;
    d->field_0x8 = src->field_0x8;
    d->field_0xc = src->field_0xc;
    d->field_0x10 = src->field_0x10;
    new (&d->set) UnkHalf_cu(static_cast<UnkHalf_cu&&>(src->set));
}

// Allocates new_capacity nodes and moves the nodes over, freeing the old array.
static inline UnkRailNode_b43a3* MoveTo(UnkRailNodes_lc09460* v, u32 new_capacity) {
    UnkRailNode_b43a3* p = static_cast<UnkRailNode_b43a3*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkRailNode_b43a3), 8, v->flags));
    if (UnkRailNode_b43a3* src = v->data) {
        UnkRailNode_b43a3* dst = p;
        for (u32 i = 0; i < v->count; ++i) {
            MoveNode(dst, src);
            src->vf_0x8();
            ++src;
            ++dst;
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100c09460(UnkRailNodes_lc09460* self, UnkRailNode_b43a3* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        UnkRailNode_b43a3* p;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = self->GrownCapacity(index);
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(self, new_capacity);
            index = self->count;
            self->data = p;
            self->capacity = new_capacity;
        } else {
            p = self->data;
        }
        MoveNode(&p[index], value);
    } else {
        if (cap <= n && self->step != 0) {
            u32 grown = self->GrownCapacity(n);
            if (cap < grown) {
                UnkRailNode_b43a3* q = MoveTo(self, grown);
                n = self->count;
                self->data = q;
                self->capacity = grown;
            }
        }
        for (u32 i = n - 1; i >= index; --i) {
            UnkRailNode_b43a3* q = &self->data[i];
            MoveNode(q + 1, q);
            self->data[i].vf_0x8();
            if (i == 0)
                break;
        }
        MoveNode(&self->data[index], value);
    }
    ++self->count;
}
