// A constructor in the CEmmyCaveAIComponent unit: the base constructor fn_710053e5c0, the vtable,
// a cleared word at +0x2a6f8, the list at +0x2a700 (fn_71004f112c), then the post-construction
// virtual call (slot 3). Names and layouts are placeholders.
#include "types.h"

// Declared as src/game/CEmmyProtoAIComponent/fn_71004faad4.cpp does.
struct UnkComponent;
void fn_710053e5c0(UnkComponent* self);
void fn_71004f112c(void* self);
extern void* g_unk_7101a43038[];

struct UnkEmmyCaveDef {
    void** vtable;
    u8 pad_0x8[0x2a6f0];
    u64 field_0x2a6f8;
    u8 field_0x2a700[0x30];
};

void fn_71004f14c0(UnkEmmyCaveDef* self) {
    fn_710053e5c0(reinterpret_cast<UnkComponent*>(self));
    self->vtable = g_unk_7101a43038 + 2;
    self->field_0x2a6f8 = 0;
    fn_71004f112c(self->field_0x2a700);
    reinterpret_cast<void (*)(UnkEmmyCaveDef*)>(self->vtable[3])(self);
}
