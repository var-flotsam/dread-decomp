// Unit 2524 (no reliable name): takes an object out of a table of frame-timed slots, from the main array
// or else from the first slot that holds it, and clears its flag. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkSlotTable_b0225 {
    u32 id;                              // 0x0
    f32 total_time;                      // 0x4
    base::global::CRntVector<void*> all;          // 0x8
    u32 slot_count;                      // 0x30
    base::global::CRntVector<void*> slots[1];     // 0x34, inline
} __attribute__((packed, aligned(4)));

// The slots in use, for a range-for.
struct UnkSlotRange_d8ba60 {
    base::global::CRntVector<void*>* first;
    base::global::CRntVector<void*>* last;
    base::global::CRntVector<void*>* begin() const { return first; }
    base::global::CRntVector<void*>* end() const { return last; }
};

static inline UnkSlotRange_d8ba60 SlotsOf(UnkSlotTable_b0225* table) {
    return {table->slots, table->slots + table->slot_count};
}

struct UnkSlotted_d8ba60 {
    u8 pad_0x0[0x3a4];
    u8 in_table;  // 0x3a4
};

// The index of `value` in `v`, or v.count when it is not there.
static inline u64 IndexOf(const base::global::CRntVector<void*>& v, void* value) {
    const u64 count = v.count;
    void* const* data = v.data;
    for (u64 i = 0; i < count; ++i) {
        if (data[i] == value)
            return i;
    }
    return count;
}

// Removes the element at `index` by moving the last one into its place.
static inline void SwapRemove(base::global::CRntVector<void*>& v, u32 index) {
    if (index < v.count) {
        u32 last = v.count - 1;
        if (index < last)
            v.data[index] = v.data[last];
        v.count = last;
    }
}

static inline bool TakeOut(base::global::CRntVector<void*>& v, void* value) {
    u64 index = IndexOf(v, value);
    if (index == v.count)
        return false;
    SwapRemove(v, index);
    return true;
}

void fn_7100d8ba60(void* holder, void* entity) {
    UnkSlotTable_b0225* table = static_cast<UnkSlotTable_b0225*>(holder);
    UnkSlotted_d8ba60* object = static_cast<UnkSlotted_d8ba60*>(entity);
    if (TakeOut(table->all, entity)) {
        object->in_table = 0;
        return;
    }
    for (auto& slot : SlotsOf(table)) {
        if (TakeOut(slot, entity)) {
            object->in_table = 0;
            return;
        }
    }
}
