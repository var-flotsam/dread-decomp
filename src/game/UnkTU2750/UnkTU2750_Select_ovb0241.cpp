// Unit 2750 of tools/tu_map.py (no reliable name): selects the sub-state a key names in a chained hash
// map (hashed by the two halves of the key's id at +0x30), unless it is already the current one and
// `flag` is clear; the previous one is told (fn_71011dcc50), the new one is made current
// (fn_71011dcc4c) and the owner's target is notified (vtable +0x1c0). Names and layouts are placeholders.
#include "types.h"

struct UnkKey_a1dcd04 {
    u8 pad_0x0[0x30];
    u32 id_lo;
    u32 id_hi;
};

struct UnkSlot_a1dcd04;

struct UnkNode_a1dcd04 {
    UnkKey_a1dcd04* key;    // 0x0
    UnkSlot_a1dcd04* slot;  // 0x8
    u8 pad_0x10[0x10];
    UnkNode_a1dcd04* next;  // 0x20
};

struct UnkTarget_a1dcd04 {
    void** vtable;

    void vf_0x1c0(bool flag) { reinterpret_cast<void (*)(UnkTarget_a1dcd04*, bool)>(vtable[0x1c0 / 8])(this, flag); }
};

struct UnkTargetHolder_a1dcd04 {
    u8 pad_0x0[0x10];
    UnkTarget_a1dcd04* target;  // 0x10
};

struct UnkSelector_a1dcd04 {
    u8 pad_0x0[8];
    UnkSlot_a1dcd04* current;  // 0x8
    u8 pad_0x10[0x10];
    UnkNode_a1dcd04** buckets;  // 0x20
    u32 count;                  // 0x28
    u32 bucket_count;           // 0x2c
    u8 pad_0x30[0x28];
    UnkTargetHolder_a1dcd04* holder;  // 0x58
};

void fn_71011dcc50(UnkSlot_a1dcd04* slot, UnkSlot_a1dcd04* next);
void fn_71011dcc4c(UnkSlot_a1dcd04* slot, UnkSlot_a1dcd04* previous);

void fn_71011dcd04(UnkSelector_a1dcd04* self, UnkKey_a1dcd04* const* key, bool flag) {
    if (self->count == 0)
        return;
    UnkKey_a1dcd04* k = *key;
    u32 hash = k->id_lo ^ k->id_hi;
    UnkNode_a1dcd04** bucket = hash % self->bucket_count + self->buckets;
    if (!bucket)
        return;
    UnkNode_a1dcd04* node = *bucket;
    if (!node)
        return;
    while (node->key != k) {
        node = node->next;
        if (!node)
            return;
    }
    UnkSlot_a1dcd04* slot = node->slot;
    if (!slot)
        return;
    UnkSlot_a1dcd04* current = self->current;
    if (current == slot && !flag)
        return;
    if (current)
        fn_71011dcc50(current, slot);
    self->current = slot;
    fn_71011dcc4c(slot, nullptr);
    self->holder->target->vf_0x1c0(flag);
}
