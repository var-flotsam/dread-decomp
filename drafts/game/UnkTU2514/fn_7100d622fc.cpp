// Unit 2514 (tools/tu_map.py, no class hint): clones a shape list: a new 0xd0-byte shape (operator new) built by
// the shape base constructor (fn_7100d6bfd8), the unit's vtable and a copy of the source's up to four shape
// pointers (count at +0xa8, pointers from +0xac). Names and layouts are placeholders.
#include "game/logic/collision/CShape.h"
#include "types.h"

extern char g_unk_7101aa8ad0[];  // The list's vtable.

struct UnkShape_ld622fc;

#pragma pack(push, 4)
struct UnkShapeList_ld622fc {
    void* vtable;
    u8 base_0x8[0xa0];  // the shape base, built by fn_7100d6bfd8
    u32 count;          // 0xa8
    UnkShape_ld622fc* items[4];  // 0xac
};
#pragma pack(pop)
static_assert(sizeof(UnkShapeList_ld622fc) == 0xcc, "layout");

UnkShapeList_ld622fc* fn_7100d622fc(const UnkShapeList_ld622fc* other) {
    auto* self = static_cast<UnkShapeList_ld622fc*>(operator new(0xd0));
    fn_7100d6bfd8(self);
    self->vtable = g_unk_7101aa8ad0 + 0x10;
    self->count = 0;
    u32 n = other->count;
    for (u32 i = 0; i < n; ++i) {
        self->items[i] = other->items[i];
        ++self->count;
    }
    return self;
}
