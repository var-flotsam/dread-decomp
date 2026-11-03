// From units 296 and 297 of tools/tu_map.py (0x710020f450-0x710020ffbc): detaches an item from the
// slot table that owns it (two notification virtuals first), moving the last item into its slot.
// Returns false when the item has no slot or belongs to another owner. Names and layouts are
// placeholders.
#include "types.h"

struct UnkUnit296Item {
    u8 pad_0x0[0x8];
    void* owner;  // 0x8
    u8 pad_0x10[0x64];
    s32 slot;  // 0x74
};

struct UnkUnit296TableBase {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(UnkUnit296Item* item);
    virtual void vf_0x50();
    virtual void vf_0x58(UnkUnit296Item* item);
};

template <u32 Slots, u32 OwnerPad>
struct UnkUnit296Table : UnkUnit296TableBase {
    UnkUnit296Item* items[Slots];  // 0x8
    u32 count;
    u8 pad[OwnerPad];
    void* owner;
};

template <typename Table>
__attribute__((always_inline)) static inline bool Detach(Table* self, UnkUnit296Item* item) {
    s32 slot = item->slot;
    if (slot < 0 || item->owner != self->owner)
        return false;
    self->vf_0x48(item);
    self->vf_0x58(item);
    item->owner = nullptr;
    u32 last = --self->count;
    UnkUnit296Item* moved = self->items[last];
    self->items[slot] = moved;
    moved->slot = slot;
    item->slot = -1;
    return true;
}

// count at 0xa008, owner at 0xa020
bool fn_710020f80c(UnkUnit296Table<0x1400, 0x14>* self, UnkUnit296Item* item) {
    return Detach(self, item);
}

// count at 0x50008, owner at 0x50018
bool fn_710020fdac(UnkUnit296Table<0xa000, 0xc>* self, UnkUnit296Item* item) {
    return Detach(self, item);
}
