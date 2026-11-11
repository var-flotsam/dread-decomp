// game::logic::collision::CPolygonCollectionShape's unit (tools/tu_map.py unit 2515). Names and layouts
// are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkTU109.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "game/logic/collision/CShape.h"
#include "types.h"

struct UnkPolygonElem_ovb0044;
struct UnkPolygonCollectionShape_b42a3;  // CPolygonCollectionShape_b42.cpp

struct UnkPolygonShape_ovb0044 {
    void* vtable;
    u8 unk_0x8[0xa0];
    u8 field_0xa8[0x40];
    bool field_0xe8;
    base::UnkVector<UnkPolygonElem_ovb0044*> field_0xf0;
};

// The copy constructor: the base shape copied, the vtable, the polygon list at +0xa8 copied, the
// flag at +0xe8, and an empty array (allocator flags 7) at +0xf0.
void fn_7100d62b68(UnkPolygonShape_ovb0044* self, const UnkPolygonShape_ovb0044* other) {
    fn_7100d6c0e4(reinterpret_cast<UnkPolygonCollectionShape_b42a3*>(self),
                  reinterpret_cast<const UnkPolygonCollectionShape_b42a3*>(other));
    self->vtable = g_unk_7101aa8b80 + 0x10;
    fn_7100123f1c(self->field_0xa8, other->field_0xa8);
    self->field_0xe8 = other->field_0xe8;
    new (&self->field_0xf0) base::UnkVector<UnkPolygonElem_ovb0044*>(7);
}

// A polygon: its point array (data, count, capacity, ..., allocator at +0x20) and the rest.
struct UnkPolygonPoints_ovb0044 {
    void* data;
    u32 count;
    u32 capacity;
    u8 unk_0x10[0x10];
    base::UnkAllocator* allocator;  // 0x20
    u8 unk_0x28[0x20];
};
static_assert(sizeof(UnkPolygonPoints_ovb0044) == 0x48, "UnkPolygonPoints_ovb0044 layout");

struct UnkPolygonShapeClear_ovb0044 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();

    u8 unk_0x8[0xa0];
    UnkPolygonPoints_ovb0044* polygons;  // 0xa8
    u32 count;                           // 0xb0
    u8 unk_0xb4[0x2c];
    bool dirty;  // 0xe0
};
static_assert(__builtin_offsetof(UnkPolygonShapeClear_ovb0044, dirty) == 0xe0, "layout");

// Frees every polygon's points, empties the list, marks it changed and notifies (vtable +0x80).
void fn_7100d633ec(UnkPolygonShapeClear_ovb0044* self) {
    for (u32 i = 0; i < self->count; ++i) {
        UnkPolygonPoints_ovb0044& p = self->polygons[i];
        if (p.data) {
            p.count = 0;
            p.allocator->vf_0x48(p.data);
            p.capacity = 0;
        }
    }
    self->dirty = true;
    self->count = 0;
    self->vf_0x80();
}
