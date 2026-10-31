// Constructor of a component in the CMagnetSurfaceHuskComponent unit: the component base
// constructor fn_71010c4148, cleared fields, the vtable (0x7101a831e0), the half-word at +0x50 set
// to 1, the member at +0x58 (fn_7100925b38), then the post-construction virtual call (slot 3).
// Names and layouts are placeholders.
#include "game/UnkComponent.h"
#include "types.h"

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
void fn_7100925b38(void* self);
extern void* g_unk_7101a831e0[];

struct UnkMagnetSurfaceHusk : UnkBaseComponent {
    u32 field_0x40;
    u64 field_0x48;
    u16 field_0x50;
    u8 pad_0x52[0x6];
    u8 field_0x58[0x8];
};

void fn_7100925c30(UnkMagnetSurfaceHusk* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->vtable = g_unk_7101a831e0 + 2;
    self->field_0x50 = 1;
    fn_7100925b38(self->field_0x58);
    reinterpret_cast<void (*)(UnkMagnetSurfaceHusk*)>(self->vtable[3])(self);
}
