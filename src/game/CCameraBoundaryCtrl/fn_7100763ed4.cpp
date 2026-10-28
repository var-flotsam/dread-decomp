// Constructor of an unidentified class: runs the base constructor, sets the vtable and clears
// its own members (a 1.0 / 0.01 pair and a flag set to 1 aside).
#include "types.h"

struct UnkBase_7100763ed4 {
    u8 unk_0x0[0x1d8];
};
void fn_71010ed044(UnkBase_7100763ed4* self);

extern void* g_unk_7101a6e6e8[];  // vtable

struct UnkStruct_7100763ed4 {
    UnkBase_7100763ed4 base;
    u64 field_0x1d8;
    u64 field_0x1e0;
    u64 field_0x1e8;
    u64 field_0x1f0;
    u64 field_0x1f8;
    u64 field_0x200;
    u64 field_0x208;
    u64 field_0x210;
    u64 field_0x218;
    u64 field_0x220;
    u64 field_0x228;
    u64 field_0x230;
    u64 field_0x238;
    u64 field_0x240;
    u64 field_0x248;
    u8 unk_0x250[4];
    f32 field_0x254;
    f32 field_0x258;
    f32 field_0x25c;
    f32 field_0x260;
    bool field_0x264;
    bool field_0x265;
    u64 field_0x268;
    u64 field_0x270;
    u64 field_0x278;
    u64 field_0x280;
    u64 field_0x288;
    bool field_0x290;
    bool field_0x291;
    u64 field_0x298;
};

void fn_7100763ed4(UnkStruct_7100763ed4* self) {
    fn_71010ed044(&self->base);
    *reinterpret_cast<void***>(self) = &g_unk_7101a6e6e8[2];
    self->field_0x1d8 = 0;
    self->field_0x1e0 = 0;
    self->field_0x1e8 = 0;
    self->field_0x1f0 = 0;
    self->field_0x1f8 = 0;
    self->field_0x200 = 0;
    self->field_0x208 = 0;
    self->field_0x210 = 0;
    self->field_0x218 = 0;
    self->field_0x220 = 0;
    self->field_0x228 = 0;
    self->field_0x230 = 0;
    self->field_0x238 = 0;
    self->field_0x240 = 0;
    self->field_0x248 = 0;
    self->field_0x254 = 0.0f;
    self->field_0x258 = 0.0f;
    self->field_0x25c = 1.0f;
    self->field_0x260 = 0.01f;
    self->field_0x264 = true;
    self->field_0x265 = false;
    self->field_0x268 = 0;
    self->field_0x270 = 0;
    self->field_0x278 = 0;
    self->field_0x280 = 0;
    self->field_0x288 = 0;
    self->field_0x290 = false;
    self->field_0x291 = false;
    self->field_0x298 = 0;
}
