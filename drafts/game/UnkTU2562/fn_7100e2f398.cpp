// CLogicDroppedItemManager::CTunableDroppableBillboard's unit: removes one billboard description from an array
// (and the matching slot of a parallel pointer array), shifting the later ones down. Names and layouts are placeholders.
#include "game/UnkTU32.h"
#include "types.h"

struct UnkBillboardDesc_le2f398 {
    u8 field_0x0[0x30];
    u8 entries[3][0x18];  // 0x30, 0x48, 0x60: id pairs
};
static_assert(sizeof(UnkBillboardDesc_le2f398) == 0x78, "placeholder layout");

struct UnkBillboardList_le2f398 {
    UnkBillboardDesc_le2f398* data;
    u64* slots;
    u32 count;
};

struct UnkBillboard_b43a3;
void fn_7100e2f4bc(UnkBillboard_b43a3* self, const UnkBillboard_b43a3* other);

static inline void ReleaseEntries(UnkBillboardDesc_le2f398* d) {
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(d->entries[2]));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(d->entries[1]));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(d->entries[0]));
}

UnkBillboardDesc_le2f398* fn_7100e2f398(UnkBillboardList_le2f398* self, UnkBillboardDesc_le2f398* pos) {
    u32 index = static_cast<u32>(pos - self->data);
    u64 i = index;
    ReleaseEntries(&self->data[i]);
    u32 last = self->count - 1;
    if (self->count >= 2 && last != index) {
        if (index < last) {
            UnkBillboardDesc_le2f398* cur = &self->data[i];
            u32 n = index;
            do {
                UnkBillboardDesc_le2f398* next = cur + 1;
                fn_7100e2f4bc(reinterpret_cast<UnkBillboard_b43a3*>(cur), reinterpret_cast<const UnkBillboard_b43a3*>(next));
                ReleaseEntries(next);
                ++n;
                cur = next;
                last = self->count - 1;
            } while (n < last);
        }
        if (index < last) {
            u64* slots = self->slots;
            u64 j = i;
            do {
                // untyped copy: the original reloads the count after each store
                __builtin_memcpy(&slots[j], &slots[j + 1], 8);
                ++j;
                last = self->count - 1;
            } while (static_cast<u32>(j) < last);
        }
    }
    self->count = last;
    return &self->data[i];
}
