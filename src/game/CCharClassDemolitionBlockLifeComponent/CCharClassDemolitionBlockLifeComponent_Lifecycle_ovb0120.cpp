// CCharClassDemolitionBlockLifeComponent's unit (tools/tu_map.py unit 1362): the constructor (fn_71008af84c;
// the base constructor fn_71008bca30, the vtable, the member at +0xa8 (fn_71008c2da8), the tunable
// defaults, then three empty string ids at +0x138..+0x148) and the destructor (fn_71008afa5c; the string
// ids, the member at +0xa8 (fn_71008c2de0), then the base destructor fn_71008bcb3c), as
// CCharClassEnemyLifeComponent's. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a7dca0[];
void fn_71008bca30(void* self);
void fn_71008c2da8(void* member);

using base::global::UnkStringData;

struct UnkDemolitionBlockLifeComponent_ovb0120 {
    void** vtable;
    u8 pad_0x8[0xa0];
    u8 field_0xa8[0x70];
    u32 field_0x118;
    f32 field_0x11c;
    f32 field_0x120;
    f32 field_0x124;
    f32 field_0x128;
    u32 field_0x12c;
    bool field_0x130;
    u8 pad_0x131[7];
    UnkStringData* field_0x138;
    UnkStringData* field_0x140;
    UnkStringData* field_0x148;
};

static inline void InitEmptyId(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_71008af84c(UnkDemolitionBlockLifeComponent_ovb0120* self) {
    fn_71008bca30(self);
    self->vtable = g_unk_7101a7dca0 + 2;
    fn_71008c2da8(self->field_0xa8);
    self->field_0x118 = 3;
    self->field_0x11c = 0.1f;
    self->field_0x120 = 0.35f;
    self->field_0x124 = 0.2f;
    self->field_0x128 = 0.5f;
    self->field_0x12c = 0;
    self->field_0x130 = false;
    InitEmptyId(&self->field_0x138);
    InitEmptyId(&self->field_0x140);
    InitEmptyId(&self->field_0x148);
}

void fn_71008c2de0(void* member);
void fn_71008bcb3c(char* self);

using base::UnkStrIdMember;

void fn_71008afa5c(char* self) {
    *reinterpret_cast<char**>(self) = reinterpret_cast<char*>(g_unk_7101a7dca0) + 0x10;
    reinterpret_cast<UnkStrIdMember*>(self + 0x148)->~UnkStrIdMember();
    reinterpret_cast<UnkStrIdMember*>(self + 0x140)->~UnkStrIdMember();
    reinterpret_cast<UnkStrIdMember*>(self + 0x138)->~UnkStrIdMember();
    fn_71008c2de0(self + 0xa8);
    fn_71008bcb3c(self);
}
