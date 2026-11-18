// Unit 161 of tools/tu_map.py (no class hint): sets up the "pp_mixer" post-process shader: its name, its
// slots, two textures, and the texture matrices and pivots (identity and zero). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkMixerShader {
    u8 pad_0x0[0xc00];
    u32 field_0xc00;
};

struct UnkRegistered_l14c890;
struct UnkEntryHolder_b0249;
void fn_710014c890(UnkRegistered_l14c890* self, const char* name);
void fn_710014ce5c(UnkEntryHolder_b0249* self, u32 kind, const CStrId* id, u32 n);
void fn_710014cb78(UnkMixerShader* self);
void fn_710014cf94(UnkMixerShader* self);
// Sets the uniform `name` to `count` vectors of four floats.
void fn_710014cbb8(UnkMixerShader* self, const CStrId* name, u32 count, const void* value);

extern CStrId g_unk_710b372ef8;
extern CStrId g_unk_710b372f00;

// A 2x4 texture matrix.
struct UnkTexMatrix {
    f32 rows[2][4];
};
extern const UnkTexMatrix g_unk_7101647d28 __attribute__((visibility("hidden")));  // identity

struct UnkTexPivot {
    f32 v[4];
};

void fn_710014f074(void* shader) {
    UnkMixerShader* self = static_cast<UnkMixerShader*>(shader);
    fn_710014c890(reinterpret_cast<UnkRegistered_l14c890*>(self), "pp_mixer");
    fn_710014cb78(self);
    fn_710014cb78(self);
    fn_710014cb78(self);
    fn_710014cf94(self);
    fn_710014cf94(self);
    fn_710014cf94(self);
    fn_710014cf94(self);
    fn_710014cf94(self);
    fn_710014cf94(self);
    fn_710014ce5c(reinterpret_cast<UnkEntryHolder_b0249*>(self), 1, &g_unk_710b372ef8, 4);
    fn_710014ce5c(reinterpret_cast<UnkEntryHolder_b0249*>(self), 1, &g_unk_710b372f00, 4);
    self->field_0xc00 = 1;
    {
        UnkTexMatrix matrix = g_unk_7101647d28;
        {
            CStrId id("uTex0Matrix", true);
            fn_710014cbb8(self, &id, 2, &matrix);
        }
        {
            CStrId id("uTex1Matrix", true);
            fn_710014cbb8(self, &id, 2, &matrix);
        }
    }
    {
        UnkTexPivot pivot = {};
        {
            CStrId id("uTex0Pivot", true);
            fn_710014cbb8(self, &id, 1, &pivot);
        }
        {
            CStrId id("uTex1Pivot", true);
            fn_710014cbb8(self, &id, 1, &pivot);
        }
    }
}
