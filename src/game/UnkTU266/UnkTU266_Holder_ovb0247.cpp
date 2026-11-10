// Unit 266 (tools/tu_map.py): a constructor of an object with a vtable, a few cleared fields, an identity
// matrix at +0x6c, two arrays (flags 8) and a helper object. Names and layouts are placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101a1ecf8[];
struct UnkAuxObject_b0247;
void fn_710020e93c(UnkAuxObject_b0247* self);

struct UnkHolder_b0247 {
    void* vtable;  // 0x0
    bool flag_0x8;
    bool flag_0x9;
    u8 pad_0xa[6];
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    u32 pad_0x24;
    u64 field_0x28;
    u32 field_0x30;
    u32 pad_0x34;
    u64 field_0x38;
    u32 field_0x40;
    u32 pad_0x44;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u64 field_0x60;
    f32 field_0x68;
    UnkMatrix44 matrix;  // 0x6c
    u8 pad_0xac[4];
    u64 field_0xb0;
    u64 field_0xb8;
    u64 field_0xc0;
    u8 field_0xc8;
    u8 pad_0xc9[7];
    u64 field_0xd0;
    base::UnkVector<void*> list_0xd8;
    base::UnkVector<void*> list_0x100;
    u8 pad_0x128[0x10];
    UnkAuxObject_b0247* aux;  // 0x138
};

void fn_71001e74bc(UnkHolder_b0247* self) {
    self->flag_0x8 = true;
    self->flag_0x9 = true;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->field_0x20 = 0;
    __builtin_memset(&self->field_0x10, 0, 16);
    __builtin_memset(&self->field_0x48, 0, 16);
    __builtin_memset(&self->field_0x58, 0, 16);
    self->field_0x68 = 1.0f;
    self->matrix = UnkMatrix44{};
    self->matrix.m[15] = 1.0f;
    self->matrix.m[10] = 1.0f;
    self->matrix.m[5] = 1.0f;
    self->matrix.m[0] = 1.0f;
    self->field_0xb0 = 0;
    self->field_0xc0 = 0;
    self->field_0xc8 = 0;
    self->vtable = g_unk_7101a1ecf8 + 0x10;
    self->field_0xd0 = 0;
    new (&self->list_0xd8) base::UnkVector<void*>(8);
    new (&self->list_0x100) base::UnkVector<void*>(8);
    UnkAuxObject_b0247* aux = static_cast<UnkAuxObject_b0247*>(fn_71000839a0(0x10, 0x38));
    fn_710020e93c(aux);
    self->aux = aux;
}
