// nn::atk unit: fills a small result struct from seven flags and two counts: which of the flag cases apply
// and the two numbers derived from them. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkWeighed {
    u8 pad_0x00[0x10];
    s32 field_0x10;
    s32 field_0x14;
    u8 pad_0x18;
    bool field_0x19;
    bool field_0x1a;
    u8 pad_0x1b[3];
    bool field_0x1e;
    u8 pad_0x1f[9];
    bool field_0x28;
};

void fn_7101213798(UnkAtkWeighed* out, bool a, bool b, bool c, bool d, bool e, bool f, s32 g, s32 h) {
    bool whole = c && f && !(d || e);
    bool rest = !whole;
    s32 base = a ? 2 : 6;
    out->field_0x1e = rest & c;
    out->field_0x28 = whole;
    out->field_0x19 = d ? rest : false;
    out->field_0x1a = e ? rest : false;
    if (whole) {
        out->field_0x10 = g;
        out->field_0x14 = base + h;
        return;
    }
    s32 value = b ? 30 : base * 2;
    value += d ? (b ? 6 : 2) : 0;
    s32 count = d ? 2 : 1;
    if (e) {
        value += b ? base : 0;
        count += 1;
    }
    out->field_0x10 = c ? count : 0;
    out->field_0x14 = c ? value : base;
}
