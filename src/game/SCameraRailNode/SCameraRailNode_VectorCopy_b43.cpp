// SCameraRailNode's unit (tools/tu_map.py): the copy constructor of an array (base::UnkVector's
// layout) of camera-rail nodes (0x58 bytes, src/game/SCameraRailNode/SCameraRailNode_b42.cpp): reset,
// copy the step, flags and allocator, reserve the other's capacity (each node moved over: its vtable,
// three words and its callback set moved with fn_71002458a4; the old one destroyed through its vf_0x8),
// then append a copy of each node (fn_7100c090cc). The shape of fn_7100246a94
// (src/game/CActor/CActor_VectorCopy_b41.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/SCameraRailNode.h"
#include "types.h"

#include <new>

// The move constructor, typed as CCentralUnitComponent's 0x40-byte half (one declaration per symbol).
struct UnkHalf_cu {
    u8 data[0x40];
    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");
};

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

struct UnkRailNodeRaw_b43a3 {
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 pad_0x14;
    UnkHalf_cu set;
};

void fn_7100c090cc(base::UnkVector<UnkRailNode_b43a3>* self, const UnkRailNode_b43a3* value);

void fn_7100c08f8c(base::UnkVector<UnkRailNode_b43a3>* self, const base::UnkVector<UnkRailNode_b43a3>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    u32 n = other->capacity;
    if (self->capacity < n) {
        UnkRailNode_b43a3* p =
            static_cast<UnkRailNode_b43a3*>(self->allocator->vf_0x40((u64)n * sizeof(UnkRailNode_b43a3), 8, self->flags));
        if (UnkRailNode_b43a3* src = self->data) {
            UnkRailNode_b43a3* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                UnkRailNodeRaw_b43a3* d = reinterpret_cast<UnkRailNodeRaw_b43a3*>(dst);
                d->vtable = g_unk_7101aa2888 + 0x10;
                d->field_0x8 = src->field_0x8;
                d->field_0xc = src->field_0xc;
                d->field_0x10 = src->field_0x10;
                new (&d->set) UnkHalf_cu(static_cast<UnkHalf_cu&&>(src->set));
                src->vf_0x8();
                ++src;
                ++dst;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (const UnkRailNode_b43a3 *it = other->data, *end = other->data + other->count; it != end; ++it)
        fn_7100c090cc(self, it);
}
