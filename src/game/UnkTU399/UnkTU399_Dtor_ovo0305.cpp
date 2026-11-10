// Unit 399 (tools/tu_map.py): a destructor: deletes the objects of the keyed table at +0x78 (and
// releases its ids), the objects of the vector at +0xa0, drops the counted resource at +0x118,
// frees the two key/value tables at +0xc8 and +0xf0, then the keyed table and the object at +0x70,
// and runs the base destructor. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkKeyedStrIds.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkStrIdMember;
extern void* g_unk_7101a21748[];
void fn_71002d5538(UnkKeyedStrIds* self);

struct UnkPolymorphic_b0249 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
};

// Two arrays (keys, values) sharing a count and an allocator.
struct UnkPlainPair_b0249 {
    void* keys;
    void* values;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;
};

struct UnkTreeOwner_b0249 {
    void** vtable;
    u8 pad_0x8[0x68];
    UnkPolymorphic_b0249* owned;  // 0x70
    UnkKeyedStrIds objects;       // 0x78
    base::UnkVector<UnkPolymorphic_b0249*> list;  // 0xa0
    UnkPlainPair_b0249 pair_a;    // 0xc8
    UnkPlainPair_b0249 pair_b;    // 0xf0
    UnkCountedResource* resource; // 0x118
};

void fn_71002df74c(UnkTreeOwner_b0249* self);

void fn_71002d52f0(UnkTreeOwner_b0249* self) {
    self->vtable = g_unk_7101a21748 + 2;
    UnkKeyedStrIds* map = &self->objects;
    UnkPolymorphic_b0249** first = static_cast<UnkPolymorphic_b0249**>(map->keys);
    for (UnkPolymorphic_b0249** p = first; p != static_cast<UnkPolymorphic_b0249**>(map->keys) + map->count; ++p) {
        if (*p) {
            (*p)->Delete();
            // the store may alias the table's fields, which are read again
            __builtin_memset(p, 0, sizeof(*p));
        }
    }
    if (map->count && map->ids) {
        for (u64 i = 0; i < map->count; ++i)
            map->ids[i].~UnkStrIdMember();
    }
    self->objects.count = 0;
    if (self->list.count) {
        for (UnkPolymorphic_b0249** p = self->list.data; p != self->list.data + self->list.count; ++p)
            if (*p)
                (*p)->Delete();
    }
    self->list.count = 0;
    if (UnkCountedResource* r = self->resource) {
        if (--r->ref_count == 0)
            fn_710001737c(r);
    }
    {
        void* values = self->pair_b.values;
        self->pair_b.count = 0;
        if (values)
            self->pair_b.allocator->vf_0x48(values);
    }
    if (self->pair_b.keys)
        self->pair_b.allocator->vf_0x48(self->pair_b.keys);
    {
        void* values = self->pair_a.values;
        self->pair_a.count = 0;
        if (values)
            self->pair_a.allocator->vf_0x48(values);
    }
    if (self->pair_a.keys)
        self->pair_a.allocator->vf_0x48(self->pair_a.keys);
    if (self->list.data) {
        self->list.count = 0;
        self->list.allocator->vf_0x48(self->list.data);
        self->list.capacity = 0;
    }
    fn_71002d5538(map);
    UnkPolymorphic_b0249* o = self->owned;
    self->owned = nullptr;
    if (o)
        o->Delete();
    fn_71002df74c(self);
}
