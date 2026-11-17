// base::tunable::CTunable's unit: constructor of a record of three string ids (made the empty
// string, creating the pool first when there is none) and a string with inline storage.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkInlineString_l133e28 {
    base::global::CRntString str;
    void** vtable;  // 0x30: the inline allocator
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;
};

struct UnkIdsInlineString_l133e28 {
    UnkStringData* ids[3];
    UnkInlineString_l133e28 str;  // 0x18
};
static_assert(__builtin_offsetof(UnkIdsInlineString_l133e28, str.field_0x140) == 0x158, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100133e28(UnkIdsInlineString_l133e28* self) {
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    fn_710007e6e4(&self->str.str, reinterpret_cast<base::UnkAllocator*>(&self->str.vtable));
    self->str.vtable = &g_unk_7101a194b8[2];
    self->str.field_0x138 = 0;
    self->str.field_0x140 = 0;
}
