// Unit 309 (tools/tu_map.py): a constructor: sets the vtable (g_unk_7101a20240 + 0x10), a point list (flags 8) that
// reserves room for ten points, and a flag. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern void* g_unk_7101a20240[];

struct UnkPointList_b0249 {
    void* vtable;
    u32 field_0x8;
    base::UnkVector<UnkVec3f> points;  // 0x10
    bool field_0x38;
};
static_assert(__builtin_offsetof(UnkPointList_b0249, field_0x38) == 0x38, "layout");

void fn_7100216dfc(UnkPointList_b0249* self) {
    self->field_0x8 = 0;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a20240) + 0x10;
    new (&self->points) base::UnkVector<UnkVec3f>(8);
    self->points.Reserve(10);
    self->field_0x38 = true;
}
