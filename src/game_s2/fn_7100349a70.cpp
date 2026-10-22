// Destructor of an array of records (count at +0x8, each record 0x48 bytes starting at +0xc and
// holding two reference-counted pointers at its start): sets its vtable, releases both pointers of
// every record and empties the array.
#include "game_s2/UnkWeakHandle.h"

extern void* g_unk_7101a23c58[];

struct __attribute__((packed)) UnkRecord {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    u8 pad_0x10[0x38];
};

struct __attribute__((packed)) UnkSelf {
    void** vtable;
    u32 field_0x8;
    UnkRecord field_0xc[1];
};

static inline void Release(UnkRefObject* p) {
    if (p && --p->ref_count == 0)
        p->vf_0x10();
}

void fn_7100349a70(UnkSelf* self) {
    self->vtable = g_unk_7101a23c58 + 2;
    for (u64 i = 0; i < self->field_0x8; i++) {
        Release(self->field_0xc[i].field_0x8);
        Release(self->field_0xc[i].field_0x0);
    }
    self->field_0x8 = 0;
}
