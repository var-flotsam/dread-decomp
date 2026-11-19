// Unit 2506 (no reliable name): appends a group of records and a refcounted handle to the paired
// arrays of the container also used by the reserve function, growing it first when full, and returns
// the new group. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkRefCountedU_ovb0244 {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
};

struct UnkSlotGroup_ovb0244 {
    u32 count;
    u8 items[128][0x70];
};

struct UnkGroupArray_ovb0244 {
    UnkSlotGroup_ovb0244* groups;
    UnkRefCountedU_ovb0244** handles;
    u32 count;
    u32 capacity;
    u32 step;
};

struct UnkGroupSource_ld4ba54 {
    u32 count;
    u8 items[128][0x70];
};

void fn_7100d4b0f4(UnkGroupArray_ovb0244* self, u32 capacity);
char* fn_7100d4b2fc(char* p0, char* p1);

UnkSlotGroup_ovb0244* fn_7100d4ba54(UnkGroupArray_ovb0244* self, UnkRefCountedU_ovb0244** handle,
                                    UnkGroupSource_ld4ba54* source) {
    u32 index = self->count;
    if (index == self->capacity) {
        fn_7100d4b0f4(self, index + self->step);
        index = self->count;
    }
    UnkRefCountedU_ovb0244** slots = self->handles;
    slots[index] = nullptr;
    if (UnkRefCountedU_ovb0244* h = *handle) {
        ++h->ref_count;
        if (UnkRefCountedU_ovb0244* prev = slots[index]) {
            if (--prev->ref_count == 0)
                prev->Delete();
        }
        slots[index] = h;
    }
    UnkSlotGroup_ovb0244* groups = self->groups;
    u64 n = self->count;
    u32 copied = 0;
    if (source->count != 0) {
        u64 i = 0;
        do {
            fn_7100d4b2fc((char*)groups[n].items[i], (char*)source->items[i]);
            copied = source->count;
            ++i;
        } while (i < copied);
    }
    groups[n].count = copied;
    u32 new_count = self->count + 1;
    self->count = new_count;
    UnkSlotGroup_ovb0244* base = self->groups;
    UnkSlotGroup_ovb0244* end = &base[new_count];
    return new_count != 0 ? end - 1 : base;
}
