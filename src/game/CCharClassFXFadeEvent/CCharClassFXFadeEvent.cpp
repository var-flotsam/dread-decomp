// A constructor in CCharClassFXFadeEvent's unit: the base constructor (fn_7100067ea4), the
// vtable, a string id built from "" and the fade's fields (two floats of 1 and two cleared flags).
// Names and the layout are placeholders.
#include "types.h"

void fn_71000003d4(void* self, const char* s, bool flag);

struct UnkFXFadeEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    u64 id_0x10;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    bool field_0x24;
    bool field_0x25;
};

void fn_7100067ea4(void* self);  // the base constructor
extern void* g_unk_7101aa5aa0[];

void fn_7100cbe13c(UnkFXFadeEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa5aa0 + 2;
    fn_71000003d4(&self->id_0x10, "", true);
    self->field_0x18 = 1.0f;
    self->field_0x1c = 0.0f;
    self->field_0x20 = 1.0f;
    self->field_0x24 = false;
    self->field_0x25 = false;
}
