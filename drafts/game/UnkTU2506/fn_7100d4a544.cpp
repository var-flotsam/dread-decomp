// UnkTU2506: copy-assigns a counted list of 0x48-byte records from another list.
// Names and layouts are placeholders.
#include "types.h"
#include "game/CAbilityShinespark.h"
#include "game/UnkTU365.h"

struct UnkAssignedSlot {
    u8 bytes[8];
};

struct UnkRecord_d4a544 {
    UnkAssignedSlot slotA;
    UnkAssignedSlot slotB;
    u32 words[13];
    u32 pad_0x44;
};

struct UnkRecordList_d4a544 {
    u32 count;
    UnkRecord_d4a544 items[1];
};

static_assert(sizeof(UnkRecord_d4a544) == 0x48, "record size");

static inline void AssignRecord(UnkRecord_d4a544* dst, const UnkRecord_d4a544* src) {
    fn_71002a5d1c(&dst->slotA, &src->slotA);
    fn_71002a5d1c(&dst->slotB, &src->slotB);
    for (int i = 0; i < 13; i++)
        dst->words[i] = src->words[i];
}

UnkRecordList_d4a544* fn_7100d4a544(UnkRecordList_d4a544* self, UnkRecordList_d4a544* other) {
    if (self == other)
        return self;
    u32 n = other->count;
    u32 oldCount = self->count;
    if (n < oldCount) {
        for (u64 i = 0; i < other->count; i++)
            AssignRecord(&self->items[i], &other->items[i]);
    } else {
        for (u64 i = 0; i < self->count; i++)
            AssignRecord(&self->items[i], &other->items[i]);
    }
    for (u64 i = self->count; i < other->count; i++) {
        fn_7100331c7c(&self->items[self->count], &other->items[i]);
        self->count++;
    }
    self->count = other->count;
    return self;
}
