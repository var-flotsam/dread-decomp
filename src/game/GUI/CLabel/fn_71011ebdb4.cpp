// Constructor of a GUI class: runs the base constructor, then initialises three arrays, two
// wide strings, a string id, a string, a list and assorted defaults.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;

struct UnkWideStr_0x20 {
    u8 data[0x20];
};

struct UnkStruct_71011ebdb4 {
    void* vtable;
    u8 unk_0x8[0x184];
    u32 field_0x18c;
    u32 field_0x190;
    u8 unk_0x194[0x4];
    base::UnkVector<void*> field_0x198[3];
    bool field_0x210;
    u8 unk_0x211[0x7];
    UnkWideStr_0x20 field_0x218;
    UnkWideStr_0x20 field_0x238;
    bool field_0x258;
    u8 unk_0x259[0x7];
    UnkStringData* field_0x260;
    u8 field_0x268[0x30];
    void* field_0x298;
    u32 unk_0x2a0;
    u32 field_0x2a4;
    u8 unk_0x2a8[0x8];
    s32 field_0x2b0;
    u32 field_0x2b4;
    u32 field_0x2b8;
    u8 unk_0x2bc[0x4];
    void* field_0x2c0;
    u32 field_0x2c8;
    u8 unk_0x2cc[0x4];
    void* field_0x2d0;
    void* field_0x2d8;
    base::UnkAllocator* field_0x2e0;
    u32 field_0x2e8;
    u8 unk_0x2ec[0x8];
    bool field_0x2f4;
    u8 unk_0x2f5[0x3];
    u32 field_0x2f8;
    u8 unk_0x2fc[0x4];
    u32 field_0x300;
    s32 field_0x304;
    s32 field_0x308;
    u8 unk_0x30c[0x8];
    u32 field_0x314;
    f32 field_0x318;
    f32 field_0x31c;
    u16 field_0x320;
    u8 unk_0x322[0x2];
    u32 field_0x324;
    u32 field_0x328;
    u8 unk_0x32c[0x4];
    bool field_0x330;
    u8 unk_0x331[0x3];
    u32 field_0x334;
    u32 field_0x338;
};

void fn_71011cce1c(UnkStruct_71011ebdb4* self);
void fn_7100066b38(UnkWideStr_0x20* self, const char* str);
void fn_710007e6c4(void* self);
extern char g_unk_7101cb6380[];
extern char g_unk_7101cb65e8[];

void fn_71011ebdb4(UnkStruct_71011ebdb4* self) {
    fn_71011cce1c(self);
    self->vtable = g_unk_7101cb6380 + 0x10;
    self->field_0x18c = 0;
    self->field_0x190 = 0;
    for (u32 i = 0; i < 3; ++i)
        new (&self->field_0x198[i]) base::UnkVector<void*>(0xf);
    self->field_0x210 = false;
    fn_7100066b38(&self->field_0x218, "");
    fn_7100066b38(&self->field_0x238, "");
    self->field_0x258 = true;
    self->field_0x260 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x260 = &g_unk_7101d47108->empty;
    fn_710007e6c4(self->field_0x268);
    self->field_0x298 = nullptr;
    self->field_0x2a4 = 2;
    self->field_0x2b0 = -1;
    self->field_0x2b4 = 1;
    self->field_0x2b8 = 0;
    self->field_0x2c0 = g_unk_7101cb65e8 + 0x10;
    self->field_0x2c8 = 0;
    self->field_0x2d0 = nullptr;
    self->field_0x2d8 = nullptr;
    self->field_0x2e0 = fn_7100083990();
    self->field_0x2e8 = 0x13;
    self->field_0x2f4 = false;
    self->field_0x2f8 = 0;
    self->field_0x300 = 0;
    self->field_0x304 = -1;
    self->field_0x308 = -1;
    self->field_0x318 = -1.0f;
    self->field_0x31c = 1.0f;
    self->field_0x314 = 0;
    self->field_0x320 = 0;
    self->field_0x324 = 0;
    self->field_0x328 = 1;
    self->field_0x330 = false;
    self->field_0x334 = 0;
    self->field_0x338 = 0;
}
