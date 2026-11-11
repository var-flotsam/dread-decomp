// The constructor (fn_71006a1cc0) of the class of translation unit 1038 (the one whose target
// list src/game/UnkScourgeTargetList.cpp reads; no reflected class of its own). The base
// constructor fn_71006f3390, the class vtable, two containers set up by fn_710059c644 (at +0x260
// and +0x2d8) around the cleared current-target handle at +0x2d0, and the float 645 at +0x258.
// Names and layouts are placeholders.
#include "game/UnkTU852.h"
#include "types.h"

void fn_71006f3390(char* self);
extern void* g_unk_7101a62938[];

struct UnkScourgeTargetOwner {
    void** vtable;
    u8 pad_0x8[0x250];
    f32 field_0x258;
    u8 pad_0x25c[4];
    u8 field_0x260[0x70];
    void* field_0x2d0;
    u8 field_0x2d8[0x8];
};

void fn_71006a1cc0(UnkScourgeTargetOwner* self) {
    fn_71006f3390(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a62938 + 2;
    fn_710059c644(self->field_0x260, 8, 0, 0x13, 0);
    self->field_0x2d0 = nullptr;
    fn_710059c644(self->field_0x2d8, 8, 0, 0x13, 0);
    self->field_0x258 = 645.0f;
}
