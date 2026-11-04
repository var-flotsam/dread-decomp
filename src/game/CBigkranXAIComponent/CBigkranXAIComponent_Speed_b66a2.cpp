// CBigkranXAIComponent unit (tools/tu_map.py, the name from reflection): writes one of two values of the
// object at +0x149b8 (+0x620 while the id at +0x149f0 is set, that is neither 0 nor 0x7fffffff, else
// +0x61c) to the output's +0x4. The two branches read the object through different types (a union of two
// views); with one type the loads merge into a csel. Names and layouts are placeholders.
#include "types.h"

struct UnkBigkranXIdle_b66a2 {
    u8 pad_0x0[0x61c];
    u32 value;
};

struct UnkBigkranXActive_b66a2 {
    u8 pad_0x0[0x620];
    u32 value;
};

struct UnkBigkranXAI_b66a2 {
    u8 pad_0x0[0x149b8];
    union {
        UnkBigkranXIdle_b66a2* idle;
        UnkBigkranXActive_b66a2* active;
    } params;
    u8 pad_0x149c0[0x30];
    s32 id;
};

struct UnkBigkranXOut_b66a2 {
    u32 field_0x0;
    u32 value;
};

void fn_71003f4cd8(UnkBigkranXAI_b66a2* self, UnkBigkranXOut_b66a2* out) {
    s32 id = self->id;
    if (id != 0 && id != 0x7fffffff) {
        out->value = self->params.active->value;
    } else {
        out->value = self->params.idle->value;
    }
}
