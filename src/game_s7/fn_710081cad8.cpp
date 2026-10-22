// Copies of fn_7100331700 (src/game_s2/fn_7100331700.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_710081cad8, fn_71008da4f0, fn_710097e418, fn_710098caac, fn_7100700d58, fn_71007bafa8.
// Destructor of an array of records (count at +0x8, each record 0x48 bytes starting at +0xc and
// holding two reference-counted pointers at its start): sets its vtable, releases both pointers of
// every record and empties the array.
#include "game_s2/UnkWeakHandle.h"

extern void* g_unk_7101a86478[];
extern void* g_unk_7101a85cd8[];
extern void* g_unk_7101a7eae0[];
extern void* g_unk_7101a75548[];
extern void* g_unk_7101a70eb0[];
extern void* g_unk_7101a6ba70[];

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

void fn_710081cad8(UnkSelf* self) {
    self->vtable = g_unk_7101a75548 + 2;
    for (u64 i = 0; i < self->field_0x8; i++) {
        Release(self->field_0xc[i].field_0x8);
        Release(self->field_0xc[i].field_0x0);
    }
    self->field_0x8 = 0;
}

void fn_71008da4f0(UnkSelf* self) {
    self->vtable = g_unk_7101a7eae0 + 2;
    for (u64 i = 0; i < self->field_0x8; i++) {
        Release(self->field_0xc[i].field_0x8);
        Release(self->field_0xc[i].field_0x0);
    }
    self->field_0x8 = 0;
}

void fn_710097e418(UnkSelf* self) {
    self->vtable = g_unk_7101a85cd8 + 2;
    for (u64 i = 0; i < self->field_0x8; i++) {
        Release(self->field_0xc[i].field_0x8);
        Release(self->field_0xc[i].field_0x0);
    }
    self->field_0x8 = 0;
}

void fn_710098caac(UnkSelf* self) {
    self->vtable = g_unk_7101a86478 + 2;
    for (u64 i = 0; i < self->field_0x8; i++) {
        Release(self->field_0xc[i].field_0x8);
        Release(self->field_0xc[i].field_0x0);
    }
    self->field_0x8 = 0;
}

void fn_7100700d58(UnkSelf* self) {
    self->vtable = g_unk_7101a6ba70 + 2;
    for (u64 i = 0; i < self->field_0x8; i++) {
        Release(self->field_0xc[i].field_0x8);
        Release(self->field_0xc[i].field_0x0);
    }
    self->field_0x8 = 0;
}

void fn_71007bafa8(UnkSelf* self) {
    self->vtable = g_unk_7101a70eb0 + 2;
    for (u64 i = 0; i < self->field_0x8; i++) {
        Release(self->field_0xc[i].field_0x8);
        Release(self->field_0xc[i].field_0x0);
    }
    self->field_0x8 = 0;
}

