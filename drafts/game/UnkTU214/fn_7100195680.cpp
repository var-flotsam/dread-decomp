// UnkTU214 (tools/tu_map.py unit 214): the constructor of a 0x140-byte child object: its base
// (fn_710019e76c), then its own vtable, a few scale and colour settings and an identity matrix.
// Names and layouts are placeholders.
#include "game/UnkTU226.h"
#include "types.h"

extern char g_unk_7101a1e018[];

struct UnkChild_l195680 {
    void* vtable;
    u8 pad_0x8[0x18];
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    u32 field_0x38;
    f32 field_0x3c[3];
    f32 field_0x48[3];
    f32 field_0x54[3];
    f32 field_0x60[3];
    f32 field_0x6c;
    u64 field_0x70[6];
    u64 field_0xa0;
    u64 field_0xa8;
    u32 field_0xb0;
    u64 field_0xb8;
    u64 field_0xc0;
    u64 field_0xc8;
    u32 field_0xd0;
    u32 field_0xd4;
    u32 field_0xd8;
    u32 field_0xdc;
    f32 field_0xe0[4];
    f32 matrix[4][4];
    u32 field_0x130;
    u16 field_0x134;
    u8 field_0x136;
    s32 field_0x138;
};
static_assert(__builtin_offsetof(UnkChild_l195680, field_0x138) == 0x138, "UnkChild_l195680 layout");

void fn_7100195680(void* self) {
    fn_710019e76c(self);
    auto* obj = static_cast<UnkChild_l195680*>(self);
    obj->vtable = g_unk_7101a1e018 + 0x10;
    obj->field_0x20 = 0;
    obj->field_0x28 = 0;
    obj->field_0x30 = 0;
    obj->field_0x38 = 0;
    obj->field_0x3c[0] = 1.0f;
    obj->field_0x3c[1] = 1.0f;
    obj->field_0x3c[2] = 1.0f;
    obj->field_0x48[0] = 0.1f;
    obj->field_0x48[1] = 0.1f;
    obj->field_0x48[2] = 0.0f;
    obj->field_0x54[0] = 0.0f;
    obj->field_0x54[1] = 1.0f;
    obj->field_0x54[2] = 1.0f;
    obj->field_0x60[0] = 1.0f;
    obj->field_0x60[1] = 1.0f;
    obj->field_0x60[2] = 1.0f;
    obj->field_0x6c = 1.0f;
    for (u64& v : obj->field_0x70)
        v = 0;
    obj->field_0xa0 = 0;
    obj->field_0xa8 = 0;
    obj->field_0xb0 = 0;
    obj->field_0xb8 = 0;
    obj->field_0xc0 = 0;
    obj->field_0xc8 = 0;
    obj->field_0xd0 = 0;
    obj->field_0xd4 = 6;
    obj->field_0xd8 = 7;
    obj->field_0xdc = 0;
    obj->field_0xe0[0] = 1.0f;
    obj->field_0xe0[1] = 1.0f;
    obj->field_0xe0[2] = 1.0f;
    obj->field_0xe0[3] = 1.0f;
    // Identity.
    obj->matrix[0][0] = 1.0f;
    obj->matrix[0][1] = 0.0f;
    obj->matrix[0][2] = 0.0f;
    obj->matrix[0][3] = 0.0f;
    obj->matrix[1][0] = 0.0f;
    obj->matrix[1][1] = 1.0f;
    obj->matrix[1][2] = 0.0f;
    obj->matrix[1][3] = 0.0f;
    obj->matrix[2][0] = 0.0f;
    obj->matrix[2][1] = 0.0f;
    obj->matrix[2][2] = 1.0f;
    obj->matrix[2][3] = 0.0f;
    obj->matrix[3][0] = 0.0f;
    obj->matrix[3][1] = 0.0f;
    obj->matrix[3][2] = 0.0f;
    obj->matrix[3][3] = 1.0f;
    obj->field_0x130 = 3;
    obj->field_0x134 = 0;
    obj->field_0x136 = 0;
    obj->field_0x138 = -1;
}
