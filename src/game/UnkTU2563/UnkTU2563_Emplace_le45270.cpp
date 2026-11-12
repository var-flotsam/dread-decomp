// The CMinimapData unit (unit 2563): appends a default bounds item under a string-id key to a keyed array
// and returns the new item. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

struct UnkEmptyBoundsItem {
    u32 field_0x0, field_0x4;
    f32 min_x, min_y;
    f32 max_x, max_y;
    UnkStringData* name;  // 0x18
    u16 field_0x20;
    u8 pad_0x22[6];
};
static_assert(sizeof(UnkEmptyBoundsItem) == 0x28, "layout");

struct UnkEmptyBoundsArray {
    UnkEmptyBoundsItem* items;
    UnkStringData** keys;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_7100e5ba88(void*, u32);

UnkEmptyBoundsItem* fn_7100e45270(UnkEmptyBoundsArray* self, const CStrId* key) {
    if (!g_unk_7101d47108)
        UnkEnsureStringPool();
    UnkStringData* empty = &g_unk_7101d47108->empty;
    if (self->count == self->capacity)
        fn_7100e5ba88(self, self->step + self->count);
    UnkStringData** slot = &self->keys[self->count];
    UnkStringData* data = key->data;
    __builtin_memcpy(slot, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
    UnkEmptyBoundsItem* item = &self->items[self->count];
    item->field_0x0 = 0;
    item->field_0x4 = 0;
    item->min_x = 3.4028235e38f;
    item->min_y = 3.4028235e38f;
    item->max_x = -3.4028235e38f;
    item->max_y = -3.4028235e38f;
    __builtin_memcpy(&item->name, &empty, sizeof(empty));
    UnkStringPool* before = g_unk_7101d47108;
    __builtin_memset(&item->field_0x20, 0, sizeof(item->field_0x20));
    u32 n = self->count + 1;
    __builtin_memcpy(&self->count, &n, sizeof(n));
    UnkEmptyBoundsItem* result = n != 0 ? &self->items[n] - 1 : self->items;
    UnkStringPool* after = g_unk_7101d47108;
    if (after && before != after) {
        if (--before->empty.ref_count == 0)
            fn_71000803a4(after, &before->empty);
    }
    return result;
}
