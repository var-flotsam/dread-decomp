// Unit 1246 (CEvacuationCountDown's, tools/tu_map.py): the constructor of a tunable with six string ids: the
// tunable base, its vtable, then each id empty. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a72d40[];

struct UnkCountDownTunable_l7d9210 : UnkTunable {
    u8 pad_0x8[0x28];
    CStrId field_0x30;
    u8 pad_0x38[0x10];
    CStrId field_0x48;
    u8 pad_0x50[0x20];
    CStrId field_0x70;
    u8 pad_0x78[0x8];
    CStrId field_0x80;
    u8 pad_0x88[0x8];
    CStrId field_0x90;
    u8 pad_0x98[0x8];
    CStrId field_0xa0;
};
static_assert(__builtin_offsetof(UnkCountDownTunable_l7d9210, field_0xa0) == 0xa0, "UnkCountDownTunable_l7d9210 layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_71007d9210(UnkCountDownTunable_l7d9210* self) {
    fn_7100136ecc(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a72d40 + 0x10);
    MakeEmpty(&self->field_0x30);
    MakeEmpty(&self->field_0x48);
    MakeEmpty(&self->field_0x70);
    MakeEmpty(&self->field_0x80);
    MakeEmpty(&self->field_0x90);
    MakeEmpty(&self->field_0xa0);
}
