// Copies of fn_710081c15c / fn_710081c770 (CGrappleBeamComponent record-list destructors): the complete and deleting
// destructors of two fixed-capacity lists in the CPowerBombMovement unit (vtables g_unk_7101a23c80, g_unk_7101a23c58):
// a u32 count at +0x8 followed by 0x48-byte records (4-byte aligned), each holding two intrusive references (count at
// +0x8, released through slot 0x10). Names and layouts are placeholders.
#include "game/UnkTU437.h"
#include "types.h"

namespace {
struct UnkPbRefObj_A5_b72 {
    void** vtable;
    s32 ref_count;
};
}  // namespace

#pragma pack(push, 4)
struct UnkPbRef_A5_b72 {
    UnkPbRefObj_A5_b72* p;
    ~UnkPbRef_A5_b72() {
        if (p && --p->ref_count == 0)
            reinterpret_cast<void (*)(UnkPbRefObj_A5_b72*)>(p->vtable[0x10 / 8])(p);
    }
};

struct UnkPbRecord_A5_b72 {
    UnkPbRef_A5_b72 first;
    UnkPbRef_A5_b72 second;
    u8 pad_0x10[0x38];
};

template <int N>
struct UnkPbRecordList_A5_b72 {
    void** vtable;
    u32 count;
    UnkPbRecord_A5_b72 items[N];  // 0xc
    u32 pad;
    u32 field_end;  // +0x2410 (128 records), +0x1210 (64 records): the count of a pointer list
};
#pragma pack(pop)
static_assert(sizeof(UnkPbRecord_A5_b72) == 0x48, "record");
static_assert(__builtin_offsetof(UnkPbRecordList_A5_b72<128>, field_end) == 0x2410, "list");
static_assert(__builtin_offsetof(UnkPbRecordList_A5_b72<64>, field_end) == 0x1210, "list");

template <int N>
static inline __attribute__((always_inline)) void Destroy_A5_b72(UnkPbRecordList_A5_b72<N>* self, void** vt) {
    self->field_end = 0;
    self->vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(vt) + 0x10);
    for (u64 i = 0; i < self->count; ++i)
        self->items[i].~UnkPbRecord_A5_b72();
    self->count = 0;
}

void fn_7100ab180c(UnkPbRecordList_A5_b72<128>* self) { Destroy_A5_b72(self, g_unk_7101a23c80); }

void fn_7100ab1b6c(UnkPbRecordList_A5_b72<64>* self) { Destroy_A5_b72(self, g_unk_7101a23c58); }

void fn_7100ab1d20(UnkPbRecordList_A5_b72<128>* self) {
    Destroy_A5_b72(self, g_unk_7101a23c80);
    __builtin_operator_delete(self);
}

void fn_7100ab2084(UnkPbRecordList_A5_b72<64>* self) {
    Destroy_A5_b72(self, g_unk_7101a23c58);
    __builtin_operator_delete(self);
}
