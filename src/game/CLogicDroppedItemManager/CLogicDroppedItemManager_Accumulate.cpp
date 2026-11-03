// CLogicDroppedItemManager::CTunableDroppableBillboard unit: two passes that run a query
// (fn_7100e2e8a4 or fn_7100e2e9b0, with 0 as the last argument) and then add the item's position
// (+0x7c) to a running sum at +0xa010, counting the items. Names and layouts are placeholders.
#include "types.h"

struct UnkDroppedItem {
    u8 unk_0x0[0x7c];
    f32 pos[3];
};

struct UnkDroppedSum {
    f32 sum[3];
    u32 count;
};

struct UnkDroppedItemManager {
    u8 unk_0x0[0xa010];
    UnkDroppedSum total;
};

void fn_7100e2e8a4(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg, u32 flags);
void fn_7100e2e9b0(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg, u32 flags);

static inline void Accumulate(UnkDroppedSum* total, const UnkDroppedItem* item) {
    f32 x = item->pos[0] + total->sum[0];
    f32 y = item->pos[1] + total->sum[1];
    f32 z = item->pos[2] + total->sum[2];
    total->sum[0] = x;
    total->sum[1] = y;
    total->sum[2] = z;
    total->count++;
}

void fn_7100e2f9a4(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg) {
    UnkDroppedSum* total = &self->total;
    fn_7100e2e9b0(self, item, arg, 0);
    Accumulate(total, item);
}

void fn_7100e2fa00(UnkDroppedItemManager* self, UnkDroppedItem* item, void* arg) {
    UnkDroppedSum* total = &self->total;
    fn_7100e2e8a4(self, item, arg, 0);
    Accumulate(total, item);
}
