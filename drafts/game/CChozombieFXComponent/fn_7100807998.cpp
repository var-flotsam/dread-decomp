// CChozombieFXComponent's unit (tools/tu_map.py, unit 1270): for each of the ten effect entries (+0x50: a
// count and the entries, which grow to the index asked for) that is not switched off, sets the alpha of
// its model's first material colour to the entry's time over the definition's duration (+0x70) and writes
// the colour to the material's two colour parameters. Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/math/UnkZeroVector3.h"
#include "types.h"

using base::math::CVector4D;

extern const base::math::CVector2D g_unk_7101647780;

// A material constant: where its value is uploaded.
struct UnkFXConstant_l807998 {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkFXMaterial_l807998 {
    u8 pad_0x0[0x14c];
    CVector4D color;  // 0x14c
    u8 pad_0x15c[0x94];
    UnkFXConstant_l807998* color_constant;  // 0x1f0
    u8 pad_0x1f8[0x48];
    CVector4D color2;  // 0x240
    u8 pad_0x250[0x90];
    UnkFXConstant_l807998* color2_constant;  // 0x2e0

    void SetColor2(const CVector4D& c) {
        fn_710007b518(&color2, &c);
        if (color2_constant)
            *color2_constant->value = c;
    }
    void SetColor(const CVector4D& c) {
        fn_710007b518(&color, &c);
        if (color_constant)
            *color_constant->value = c;
    }
};
static_assert(__builtin_offsetof(UnkFXMaterial_l807998, color2_constant) == 0x2e0, "UnkFXMaterial_l807998 layout");

struct UnkFXMesh_l807998 {
    u8 pad_0x0[0x48];
    UnkFXMaterial_l807998** materials;  // 0x48
};

struct UnkFXModelData_l807998 {
    u8 pad_0x0[0x78];
    UnkFXMesh_l807998* mesh;  // 0x78
};

struct UnkFXModel_l807998 {
    u8 pad_0x0[0x138];
    UnkFXModelData_l807998* data;  // 0x138

    UnkFXMaterial_l807998* Material() { return data->mesh->materials[0]; }
};

// One effect entry; the original lays these out 4-aligned after the count.
struct __attribute__((packed, aligned(4))) UnkChozombieFXEntry_l807998 {
    UnkFXModel_l807998* model;          // 0x0
    base::math::CVector3D field_0x8;    // 0x8
    base::math::CVector3D field_0x14;   // 0x14
    base::math::CVector2D field_0x20;   // 0x20
    f32 time;                           // 0x28
    u8 pad_0x2c[0x4];
    u64 field_0x30;                     // 0x30
    bool off;                           // 0x38
    u8 pad_0x39[0x7];

    UnkChozombieFXEntry_l807998()
        : model(nullptr),
          field_0x8{g_unk_710164779c.x, g_unk_710164779c.y, g_unk_710164779c.z},
          field_0x14{g_unk_710164779c.x, g_unk_710164779c.y, g_unk_710164779c.z},
          field_0x20{g_unk_7101647780.x, g_unk_7101647780.y}, time(0.0f), field_0x30(0), off(true) {}
};
static_assert(sizeof(UnkChozombieFXEntry_l807998) == 0x40, "UnkChozombieFXEntry_l807998 layout");

struct UnkChozombieFXEntries_l807998 {
    u32 count;
    UnkChozombieFXEntry_l807998 data[10];

    // The entry at `index`, constructing those up to it first.
    UnkChozombieFXEntry_l807998& At(u64 index) {
        if (index >= count) {
            for (u64 i = count; i <= index; ++i)
                new (&data[i]) UnkChozombieFXEntry_l807998();
            count = index + 1;
        }
        return data[index];
    }
};

struct UnkChozombieFXDef_l807998 {
    u8 pad_0x0[0x70];
    f32 duration;  // 0x70
};

struct UnkChozombieFX_l807998 {
    u8 pad_0x0[0x10];
    UnkChozombieFXDef_l807998* def;  // 0x10
    u8 pad_0x18[0x38];
    UnkChozombieFXEntries_l807998 entries;  // 0x50
};
static_assert(__builtin_offsetof(UnkChozombieFX_l807998, entries) == 0x50, "UnkChozombieFX_l807998 layout");

void fn_7100807998(UnkChozombieFX_l807998* self) {
    UnkChozombieFXDef_l807998* def = self->def;
    for (u64 i = 0; i != 10; ++i) {
        if (self->entries.At(i).off)
            continue;
        f32 alpha = self->entries.At(i).time / def->duration;
        UnkFXMaterial_l807998* material = self->entries.At(i).model->Material();
        CVector4D color = {material->color.x, material->color.y, material->color.z, alpha};
        self->entries.At(i).model->Material()->SetColor2(color);
        self->entries.At(i).model->Material()->SetColor(color);
    }
}
