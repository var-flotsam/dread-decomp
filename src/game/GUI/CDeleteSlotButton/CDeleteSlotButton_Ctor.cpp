// GUI::CDeleteSlotButton's constructor (the name from reflection): GUI::CButton's (fn_71011bf0ec),
// then a press callback bound to the button (fn_7100b0102c), no slot (-1), and the button's set-up
// (fn_71011cd128). Names and layouts are placeholders.
#include "game/GUI/CButton.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

extern char g_unk_7101a99c90[];  // CDeleteSlotButton's vtable
struct UnkGuiButton;
void fn_7100b0102c();              // the bound callback

struct UnkDeleteSlotButton {
    char* vtable;
    u8 base_0x8[0x2a0];
    void* field_0x2a8;  // a callback: unused word, the object, the function
    void* object;
    void (*fn)();
    s32 slot;  // 0x2c0
};
static_assert(__builtin_offsetof(UnkDeleteSlotButton, slot) == 0x2c0, "UnkDeleteSlotButton layout");

void fn_7100b00fd0(UnkDeleteSlotButton* self) {
    fn_71011bf0ec(reinterpret_cast<UnkGuiButton*>(self));
    self->vtable = g_unk_7101a99c90 + 0x10;
    self->field_0x2a8 = nullptr;
    self->object = self;
    self->fn = fn_7100b0102c;
    self->slot = -1;
    fn_71011cd128(reinterpret_cast<char*>(self));
}
