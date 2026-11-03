// CBreakableTileGroupComponent unit (tools/tu_map.py): the moving "set element at index" function
// (slot 0x148 of the reflected collection type) of an array (base::UnkVector's layout) of the 0x48-byte
// records of CBreakableTileGroupComponent_b40.cpp; a copy of fn_7100da11e4
// (src/game/CLightsDef/CLightsDef_b42.cpp). An index past the end of a growable array first reserves
// its capacity (grown to the next multiple of the step when the index reaches it, fn_71007613d8) and
// then index + 1 elements, constructs the new ones (fn_71007582a4) and extends the count; then the
// element is move-assigned (fn_7100758388). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// Defined in src/game/CBreakableTileGroupComponent/CBreakableTileGroupComponent_b40.cpp.
struct UnkTileRecord_bt;
UnkTileRecord_bt* fn_7100758388(UnkTileRecord_bt* self, UnkTileRecord_bt* other);

struct UnkTileRecordElem_b43a2 {
    u8 unk_0x0[0x48];
};

struct UnkTileRecordArray_b43a2 {
    UnkTileRecordElem_b43a2* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_71007613d8(UnkTileRecordArray_b43a2* self, u32 capacity);
void fn_71007582a4(UnkTileRecordElem_b43a2* self);

static inline void Grow_b43a2(UnkTileRecordArray_b43a2* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_71007613d8(self, capacity);
    u32 n = index + 1;
    fn_71007613d8(self, n);
    for (u32 i = self->count; i < n; ++i)
        fn_71007582a4(&self->data[i]);
    self->count = n;
}

void fn_710075e39c(UnkTileRecordArray_b43a2* self, const u32* index, UnkTileRecordElem_b43a2* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b43a2(self, i);
    fn_7100758388(reinterpret_cast<UnkTileRecord_bt*>(&self->data[i]), reinterpret_cast<UnkTileRecord_bt*>(value));
}
