// A constructor in CCharClassSetMaterialPropertyTransitionEvent's unit: the base constructor
// (fn_7100067ea4), the vtable, a string id built from "" and the value fields, ending in two
// floats of 1 (as the FX material events in
// src/game/CCharClassFXSetMaterialPropertyEvent/CCharClassFXSetMaterialPropertyEvent.cpp, with one
// id). Names and the layout are placeholders.
#include "types.h"

void fn_71000003d4(void* self, const char* s, bool flag);

struct UnkMaterialTransitionEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    u64 id_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
    u64 field_0x28;
    f32 field_0x30;
    f32 field_0x34;
};

void fn_7100067ea4(void* self);  // the base constructor
extern void* g_unk_7101aa6138[];

void fn_7100cd5c4c(UnkMaterialTransitionEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa6138 + 2;
    fn_71000003d4(&self->id_0x10, "", true);
    self->field_0x18 = 0;
    self->field_0x1c = 1;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x30 = 1.0f;
    self->field_0x34 = 1.0f;
}
