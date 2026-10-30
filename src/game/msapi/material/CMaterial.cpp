// From unit 229 of tools/tu_map.py (msapi::material::CMaterial and its state types): restoring parts
// of a material's render state from the copy kept at +0xc0, one group per flag bit. The groups'
// meanings are not known; names and layouts are placeholders.
#include "types.h"

// The render state (0x90 bytes; the live one at +0x30, the saved one at +0xc0).
struct UnkMaterialState {
    u64 field_0x0[2];  // bit 1
    u8 pad_0x10[0x8];
    u32 field_0x18;  // bit 2
    u8 pad_0x1c[0xc];
    u64 field_0x28[4];  // bit 3
    u8 pad_0x48[0x8];
    u64 field_0x50;  // bit 0
    u32 field_0x58;
    u8 pad_0x5c[0xc];
    u32 field_0x68;  // bit 5
    u8 pad_0x6c[0xc];
    u64 field_0x78;  // bit 4
    u8 field_0x80;
    u8 pad_0x81[0x7];
    u32 field_0x88;  // bit 8 restores the material's +0x18 from it
    u8 pad_0x8c[0x4];
};
static_assert(sizeof(UnkMaterialState) == 0x90, "UnkMaterialState layout");

struct UnkMaterial {
    u8 pad_0x0[0x18];
    u32 field_0x18;
    u8 pad_0x1c[0x14];
    UnkMaterialState state;  // 0x30
    UnkMaterialState saved;  // 0xc0
};
static_assert(__builtin_offsetof(UnkMaterial, saved) == 0xc0, "UnkMaterial layout");

void fn_71001a6ec8(UnkMaterial* self, u32 flags) {
    if (flags & 2) {
        self->state.field_0x0[0] = self->saved.field_0x0[0];
        self->state.field_0x0[1] = self->saved.field_0x0[1];
    }
    if (flags & 4)
        self->state.field_0x18 = self->saved.field_0x18;
    if (flags & 8) {
        self->state.field_0x28[0] = self->saved.field_0x28[0];
        self->state.field_0x28[1] = self->saved.field_0x28[1];
        self->state.field_0x28[2] = self->saved.field_0x28[2];
        self->state.field_0x28[3] = self->saved.field_0x28[3];
    }
    if (flags & 1) {
        self->state.field_0x50 = self->saved.field_0x50;
        self->state.field_0x58 = self->saved.field_0x58;
    }
    if (flags & 0x20)
        self->state.field_0x68 = self->saved.field_0x68;
    if (flags & 0x10) {
        self->state.field_0x78 = self->saved.field_0x78;
        self->state.field_0x80 = self->saved.field_0x80;
    }
    if (flags & 0x100)
        self->field_0x18 = self->saved.field_0x88;
}
