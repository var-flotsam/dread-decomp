// Constructor of a class with two vtables (unk_7101a1e508 at 0 and its second table at 0x20)
// above the base built by fn_710019e76c: clears the fields after the base. Called by
// fn_7100169e28.

#include "types.h"

extern char unk_7101a1e508[];
void fn_710019e76c(void* self);

struct UnkObject_71001a9f84 {
    char* vtable;
    u8 base_0x8[0x18];
    char* vtable_0x20;
    u64 field_0x28;
    u32 field_0x30;
    u64 field_0x38;
    u32 field_0x40;
    u64 field_0x48;
    bool field_0x50;
};

void fn_71001a9f84(UnkObject_71001a9f84* self) {
    fn_710019e76c(self);
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x28 = 0;
    self->vtable = unk_7101a1e508 + 0x10;
    self->vtable_0x20 = unk_7101a1e508 + 0x40;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x50 = false;
}
