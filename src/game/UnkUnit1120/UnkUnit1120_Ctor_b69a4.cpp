// From unit 1120 of tools/tu_map.py (0x710070104c-0x71007019ec, no reflected class): fn_710070104c, the
// constructor of an object built on the unit 1124 object (fn_7100703814, which takes the id), with an
// embedded kind-7 parameter block (vtable g_unk_7101a265a8, layout as game_s2/UnkParams7.h) at +0xb0, the
// path query of unit 1130 (fn_7100706818) at +0x150 and a few cleared fields after it. Names and layouts
// are placeholders.
#include "game/UnkTU1124.h"
#include "game_s2/UnkParams7.h"
#include "types.h"

struct UnkUnit1124Object;
struct UnkSelf_nq41;
void fn_7100706818(UnkSelf_nq41* self, void* owner);

extern void* g_unk_7101a6ba98[];  // the object's vtable
// g_unk_7101a265a8 (the parameter block's vtable) is declared in game_s2/UnkParams7.h.

struct UnkParamsBlock_b69a4 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    void* field_0x38;
    u8 field_0x40;
    u8 pad_0x41[7];
    void* field_0x48;
    u8 field_0x50;
    u8 pad_0x51[7];
    u8 field_0x58;
    u8 pad_0x59;
    u8 field_0x5a;
    u8 pad_0x5b;
    u8 field_0x5c;
    u8 pad_0x5d;
    u8 field_0x5e;
    u8 pad_0x5f[5];
    u8 field_0x64;
    u8 pad_0x65[3];
    u64 field_0x68[2];
    u8 field_0x78;
    u8 pad_0x79[7];
    u64 field_0x80[2];
    u8 field_0x90;
    u8 pad_0x91[7];
    u8 field_0x98;
    u8 pad_0x99[7];
};
static_assert(sizeof(UnkParamsBlock_b69a4) == 0xa0, "UnkParamsBlock_b69a4 size");

struct UnkUnit1120Object_b69a4 {
    void** vtable;
    u8 base_0x8[0xa8];
    UnkParamsBlock_b69a4 params;  // 0xb0
    u8 query[0x38040];            // 0x150
    s32 field_0x38190;
    u8 pad_0x38194[4];
    u64 field_0x38198[4];
    bool field_0x381b8;
};

void fn_710070104c(UnkUnit1120Object_b69a4* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    self->vtable = &g_unk_7101a6ba98[2];
    UnkParamsBlock_b69a4& p = self->params;
    p.field_0x1c = 0;
    __builtin_memset(p.field_0x8, 0, sizeof(p.field_0x8));
    p.field_0x20[0] = 0;
    p.field_0x20[1] = 0;
    p.field_0x30 = 0;
    p.vtable = &g_unk_7101a265a8[2];
    p.field_0x40 = 0;
    p.field_0x50 = 0;
    p.field_0x58 = 0;
    p.field_0x5a = 0;
    p.field_0x5c = 0;
    p.field_0x5e = 0;
    p.field_0x64 = 0;
    p.field_0x98 = 0;
    p.field_0x78 = 0;
    p.field_0x90 = 0;
    p.field_0x34 = 7;
    p.field_0x68[0] = 0;
    p.field_0x68[1] = 0;
    p.field_0x80[0] = 0;
    p.field_0x80[1] = 0;
    fn_7100706818(reinterpret_cast<UnkSelf_nq41*>(self->query), self);
    self->field_0x38190 = -1;
    self->field_0x381b8 = false;
    for (u64& v : self->field_0x38198)
        v = 0;
}
