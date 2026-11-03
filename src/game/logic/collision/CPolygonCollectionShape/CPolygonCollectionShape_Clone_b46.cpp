// game::logic::collision::CPolygonCollectionShape's unit (tools/tu_map.py unit 2515): a clone: a
// new 0x120-byte shape (operator new, fn_7100083b60) built as the reflected copy constructor fn_7100d665c8 does
// (CPolygonCollectionShape_b42.cpp): the base shape copied (fn_7100d6c0e4), the vtable, the member
// at +0xa8 copied (fn_7100123f1c), the flag at +0xe8 and an empty array (allocator flags 7) at
// +0xf0. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkPolygonElem_b46a3;

struct UnkPolygonShape_b46a3 {
    void* vtable;
    u8 unk_0x8[0xa0];
    u8 field_0xa8[0x40];
    bool field_0xe8;
    base::UnkVector<UnkPolygonElem_b46a3*> field_0xf0;
};

struct UnkPolygonCollectionShape_b42a3;  // CPolygonCollectionShape_b42.cpp
void fn_7100d6c0e4(UnkPolygonCollectionShape_b42a3* self, const UnkPolygonCollectionShape_b42a3* other);
void fn_7100123f1c(void* self, const void* other);
extern char g_unk_7101aa8b80[];

UnkPolygonShape_b46a3* fn_7100d6350c(const UnkPolygonShape_b46a3* other) {
    auto* self = static_cast<UnkPolygonShape_b46a3*>(__builtin_operator_new(0x120));
    fn_7100d6c0e4(reinterpret_cast<UnkPolygonCollectionShape_b42a3*>(self),
                  reinterpret_cast<const UnkPolygonCollectionShape_b42a3*>(other));
    self->vtable = g_unk_7101aa8b80 + 0x10;
    fn_7100123f1c(self->field_0xa8, other->field_0xa8);
    self->field_0xe8 = other->field_0xe8;
    new (&self->field_0xf0) base::UnkVector<UnkPolygonElem_b46a3*>(7);
    return self;
}
