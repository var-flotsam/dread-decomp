// GUI::CButton's constructor (unit 2745 of tools/tu_map.py, the name from reflection): the display
// object container's (fn_71011d8d24), then four empty string ids, a callback bound to the button
// (fn_71011c14e0) and the rest cleared. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101cb5908[];  // CButton's vtable
void fn_71011d8d24(void*);       // the container's constructor
void fn_71011c14e0();            // the bound callback

struct UnkGuiButton {
    char* vtable;
    u8 base_0x8[0x228];
    CStrId field_0x230;
    CStrId field_0x238;
    CStrId field_0x240;
    CStrId field_0x248;
    u16 field_0x250;
    u8 pad_0x252[0x6];
    void* field_0x258;  // a callback: unused word, the object, the function
    void* object;
    void (*fn)();
    u64 field_0x270;
    u64 field_0x278;
    u64 field_0x280;
    bool field_0x288;
    u8 pad_0x289[0x3];
    u8 field_0x28c[0x15];  // cleared
};
static_assert(__builtin_offsetof(UnkGuiButton, field_0x28c) == 0x28c, "UnkGuiButton layout");

void fn_71011bf0ec(UnkGuiButton* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101cb5908 + 0x10;
    new (&self->field_0x230) CStrId("", true);
    new (&self->field_0x238) CStrId("", true);
    new (&self->field_0x240) CStrId("", true);
    new (&self->field_0x248) CStrId("", true);
    self->field_0x250 = 0;
    self->field_0x258 = nullptr;
    self->object = self;
    self->fn = fn_71011c14e0;
    self->field_0x270 = 0;
    self->field_0x278 = 0;
    self->field_0x280 = 0;
    self->field_0x288 = true;
    __builtin_memset(self->field_0x28c, 0, sizeof(self->field_0x28c));
}
