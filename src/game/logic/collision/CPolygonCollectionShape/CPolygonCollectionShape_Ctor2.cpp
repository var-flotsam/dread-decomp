// game::logic::collision::CPolygonCollectionShape unit: a second constructor of the shape (as
// fn_7100d62b04 in src/game/logic/collision/CPolygonCollectionShape.cpp): the shape base
// (fn_7100d6bfd8), the vtable, the polygon list at +0xa8 (fn_7100123eb4), a cleared flag and an empty
// array with allocation flags 7. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkTileOutlineList {
    u8 data[0x40];
};
void fn_7100123eb4(UnkTileOutlineList* self);

struct UnkPolygonCollectionShapeView2 {
    char* vtable;
    u8 base_0x8[0xa0];
    UnkTileOutlineList outlines;
    bool field_0xe8;
    base::UnkVector<u64> field_0xf0;
};
static_assert(__builtin_offsetof(UnkPolygonCollectionShapeView2, field_0xf0) == 0xf0, "layout");

extern char g_unk_7101aa8b80[];
void fn_7100d6bfd8(void* self);

void fn_7100d66568(void* block) {
    UnkPolygonCollectionShapeView2* self = static_cast<UnkPolygonCollectionShapeView2*>(block);
    fn_7100d6bfd8(self);
    self->vtable = g_unk_7101aa8b80 + 0x10;
    fn_7100123eb4(&self->outlines);
    self->field_0xe8 = false;
    new (&self->field_0xf0) base::UnkVector<u64>(7);
}
