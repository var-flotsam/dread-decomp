// Unit 99 (CTunableSoundSystemATK): grows the section table to cover the current index and records a size.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSection {
    u8 pad_0x00[0x10];
    s32 start;
    s32 end;
    s32 field_0x18;
    u8 pad_0x1c[0x04];
};

struct UnkSource;

struct UnkSectionTable {
    u32 count;
    UnkSection items[1];
    u8 pad_0x24[0x104 - 0x24];
    s32 current;

    void Grow(u32 needed) {
        if (needed > count) {
            __builtin_memset(&items[count], 0, (u64)needed * sizeof(UnkSection) - (u64)count * sizeof(UnkSection));
            count = needed;
        }
    }
};

struct UnkTunable {
    u8 pad_0x00[0x28];
    UnkSource** source;
    u8 pad_0x30[0x08];
    UnkSectionTable table;
};

extern s32 fn_710121e6b4(UnkSource* source);

void fn_71000f4c10(UnkTunable* self) {
    UnkSource* source = *self->source;
    if (!source || self->table.current < 0)
        return;
    s32 size = fn_710121e6b4(source) + 1;
    u32 index = self->table.current;
    self->table.Grow(index + 1);
    self->table.items[index].field_0x18 = size;
}
