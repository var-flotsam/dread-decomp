// base::reflection unit 1634 (tools/tu_map.py): a constructor: cleared flags, three empty strings,
// four empty string ids, another empty string, and a mask of all thirteen bits taken from a
// guarded static. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkMaskZero_lad8550 {
    u32 value;
    UnkMaskZero_lad8550() : value(0) {}
};

inline const UnkMaskZero_lad8550& UnkMaskNone_lad8550() {
    static UnkMaskZero_lad8550 s_value asm("g_unk_7172758220");
    return s_value;
}

inline u32 UnkMaskAll_lad8550() {
    static u32 s_mask asm("g_unk_7172758210") = ~UnkMaskNone_lad8550().value & 0x1fff;
    return s_mask;
}

struct UnkObject_lad8550 {
    bool field_0x0;
    bool field_0x1;
    u8 pad_0x2[6];
    u64 field_0x8;
    bool field_0x10;
    u8 pad_0x11[7];
    u8 strings[3][0x30];  // 0x18, CRntString
    UnkStringData* ids[4];  // 0xa8
    u64 field_0xc8;
    u32 field_0xd0;
    u32 field_0xd4;
    bool field_0xd8;
    bool field_0xd9;
    u8 pad_0xda[6];
    u8 string_0xe0[0x30];  // CRntString
    u32 field_0x110;
    u32 mask;  // 0x114
    u64 field_0x118;
    u32 field_0x120;
};
static_assert(__builtin_offsetof(UnkObject_lad8550, field_0x120) == 0x120, "layout");

static inline void MakeEmpty(UnkStringData** id) {
    *id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    *id = &g_unk_7101d47108->empty;
}

void fn_7100ad8550(UnkObject_lad8550* self) {
    self->field_0x0 = false;
    self->field_0x1 = false;
    self->field_0x8 = 0;
    self->field_0x10 = false;
    fn_710007e6c4(self->strings[0]);
    fn_710007e6c4(self->strings[1]);
    fn_710007e6c4(self->strings[2]);
    MakeEmpty(&self->ids[0]);
    MakeEmpty(&self->ids[1]);
    MakeEmpty(&self->ids[2]);
    MakeEmpty(&self->ids[3]);
    self->field_0xc8 = 0;
    self->field_0xd0 = 0;
    self->field_0xd4 = 1;
    self->field_0xd8 = false;
    self->field_0xd9 = true;
    fn_710007e6c4(self->string_0xe0);
    self->field_0x110 = 0;
    self->mask = UnkMaskAll_lad8550();
    self->field_0x118 = 0;
    self->field_0x120 = 0;
}
