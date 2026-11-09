// CCharClassEnemyLifeComponent's unit (tools/tu_map.py unit 1366): the constructor (fn_71008b4544; the
// base constructor fn_71008bca30, the vtable 0x7101a7de00, the member at +0xa8 (fn_71008c2da8), the flag
// at +0x118, 0.01 at +0x11c, the two empty string ids at +0x120 and +0x128, then cleared words) and the
// destructor (fn_71008b46cc; the string ids, the member at +0xa8 (fn_71008c2de0), then the base
// destructor fn_71008bcb3c), as CCharClassItemLifeComponent's and CCharClassEmmyAIComponent's. Names
// and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a7de00[];
void fn_71008bca30(void* self);
void fn_71008c2da8(void* member);

using base::global::UnkStringData;

struct UnkEnemyLifeComponent_ovb0090 {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 field_0xa8[0x70];
    bool field_0x118;
    u8 pad_0x119[3];
    f32 field_0x11c;
    UnkStringData* field_0x120;
    UnkStringData* field_0x128;
    u64 field_0x130;
    u16 field_0x138;
    u8 pad_0x13a[2];
    u32 field_0x13c[3];
    bool field_0x148;
};

static inline void InitEmptyId(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71008b4544(UnkEnemyLifeComponent_ovb0090* self) {
    fn_71008bca30(self);
    self->vtable = g_unk_7101a7de00 + 2;
    fn_71008c2da8(self->field_0xa8);
    self->field_0x118 = true;
    self->field_0x11c = 0.01f;
    InitEmptyId(&self->field_0x120);
    InitEmptyId(&self->field_0x128);
    __builtin_memset(self->field_0x13c, 0, sizeof(self->field_0x13c));
    self->field_0x130 = 0;
    self->field_0x138 = 0;
    self->field_0x148 = true;
}

void fn_71008c2de0(void* member);
void fn_71008bcb3c(char* self);

using base::UnkStrIdMember;

void fn_71008b46cc(char* self) {
    *reinterpret_cast<char**>(self) = reinterpret_cast<char*>(g_unk_7101a7de00) + 0x10;
    reinterpret_cast<UnkStrIdMember*>(self + 0x128)->~UnkStrIdMember();
    reinterpret_cast<UnkStrIdMember*>(self + 0x120)->~UnkStrIdMember();
    fn_71008c2de0(self + 0xa8);
    fn_71008bcb3c(self);
}
