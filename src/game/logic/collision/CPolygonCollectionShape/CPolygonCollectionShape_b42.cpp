// game::logic::collision::CPolygonCollectionShape's unit (tools/tu_map.py): its reflected copy and move
// constructors. Each copies the base shape (fn_7100d6c0e4), sets the vtable, copies or moves the
// member at +0xa8 (fn_7100123f1c, fn_7100123f58), copies the flag at +0xe8 and starts an empty array
// (allocator flags 7) at +0xf0 instead of copying it. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkPolygonElem_b42a3;

struct UnkPolygonCollectionShape_b42a3 {
    void* vtable;
    u8 unk_0x8[0xa0];
    u8 field_0xa8[0x40];
    bool field_0xe8;
    base::UnkVector<UnkPolygonElem_b42a3*> field_0xf0;
};

void fn_7100d6c0e4(UnkPolygonCollectionShape_b42a3* self, const UnkPolygonCollectionShape_b42a3* other);
void fn_7100123f1c(void* self, const void* other);
void fn_7100123f58(void* self, void* other);
extern char g_unk_7101aa8b80[];

// Signatures as src/reflection/game_logic_collision_CPolygonCollectionShape.cpp declares them.
void fn_7100d665c8(void* dst, const void* src) {
    auto* self = static_cast<UnkPolygonCollectionShape_b42a3*>(dst);
    auto* other = static_cast<const UnkPolygonCollectionShape_b42a3*>(src);
    fn_7100d6c0e4(self, other);
    self->vtable = g_unk_7101aa8b80 + 0x10;
    fn_7100123f1c(self->field_0xa8, other->field_0xa8);
    self->field_0xe8 = other->field_0xe8;
    new (&self->field_0xf0) base::UnkVector<UnkPolygonElem_b42a3*>(7);
}

void fn_7100d66634(void* dst, const void* src) {
    auto* self = static_cast<UnkPolygonCollectionShape_b42a3*>(dst);
    auto* other = static_cast<UnkPolygonCollectionShape_b42a3*>(const_cast<void*>(src));
    fn_7100d6c0e4(self, other);
    self->vtable = g_unk_7101aa8b80 + 0x10;
    fn_7100123f58(self->field_0xa8, other->field_0xa8);
    self->field_0xe8 = other->field_0xe8;
    new (&self->field_0xf0) base::UnkVector<UnkPolygonElem_b42a3*>(7);
}
