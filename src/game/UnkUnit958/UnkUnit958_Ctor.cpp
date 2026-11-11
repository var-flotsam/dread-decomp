// The constructor (fn_7100639cf8) of the class of translation unit 958 (no reflected class; built
// at +0x80 of the navigation component, src/game/CAIManager/CTunableAIManager/fn_7100da7e2c.cpp):
// its vtable, an empty array at +0x8 (allocator flags 0xf) reserved for 6 entries (fn_710063cb28),
// a container at +0x30 set up by fn_710059c644, and cleared fields at +0xa0 and +0xb0. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU852.h"
#include "game/UnkTU958.h"
#include "types.h"

struct UnkUnit958Entry;

struct UnkUnit958Array {
    UnkUnit958Entry* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710063cb28(UnkUnit958Array* array, u32 n);

struct UnkUnit958 {
    void** vtable;
    UnkUnit958Array entries;
    u8 field_0x30[0x70];
    u64 field_0xa0;
    u64 field_0xa8;
    bool field_0xb0;
};

void fn_7100639cf8(UnkUnit958* self) {
    self->entries.count = 0;
    self->entries.capacity = 0;
    self->vtable = g_unk_7101a58cd8 + 2;
    self->entries.data = nullptr;
    self->entries.step = 10;
    self->entries.field_0x14 = true;
    self->entries.flags = 0xf;
    self->entries.allocator = fn_7100083990();
    fn_710063cb28(&self->entries, 6);
    fn_710059c644(self->field_0x30, 10, 0, 0x13, 0);
    self->field_0xb0 = false;
    self->field_0xa0 = 0;
    self->field_0xa8 = 0;
}
