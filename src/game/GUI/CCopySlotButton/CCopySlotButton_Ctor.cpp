// GUI::CCopySlotButton's constructor (the name from reflection): GUI::CButton's (fn_71011bf0ec),
// then a press callback bound to the button (fn_7100afb80c), no slot (-1), and the button's set-up
// (fn_71011cd128). Names and layouts are placeholders.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

extern char g_unk_7101a99010[];  // CCopySlotButton's vtable
struct UnkGuiButton;
void fn_71011bf0ec(UnkGuiButton* self);  // CButton's constructor
void fn_7100afb80c();              // the bound callback

struct UnkCopySlotButton {
    char* vtable;
    u8 base_0x8[0x2a0];
    void* field_0x2a8;  // a callback: unused word, the object, the function
    void* object;
    void (*fn)();
    s64 slot;  // 0x2c0
};
static_assert(__builtin_offsetof(UnkCopySlotButton, slot) == 0x2c0, "UnkCopySlotButton layout");

void fn_7100afb7b0(UnkCopySlotButton* self) {
    fn_71011bf0ec(reinterpret_cast<UnkGuiButton*>(self));
    self->vtable = g_unk_7101a99010 + 0x10;
    self->field_0x2a8 = nullptr;
    self->object = self;
    self->fn = fn_7100afb80c;
    self->slot = -1;
    fn_71011cd128(reinterpret_cast<char*>(self));
}
