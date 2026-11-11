// CGrappleBeamComponent unit (1277): the destructors (complete and deleting) of a fixed-capacity list (vtable g_unk_7101a75548): a u32
// count at +0x8 followed directly by 0x48-byte records (4-byte aligned), each holding two intrusive references
// (count at +0x8, released through slot 0x10). The second reference of a record goes first. Names and layouts
// are placeholders.
#include "game/CGrappleBeamComponent.h"
#include "types.h"

struct UnkGrappleRefObj_A5_b70 {
    void** vtable;
    s32 ref_count;
};

#pragma pack(push, 4)
struct UnkGrappleRef_A5_b70 {
    UnkGrappleRefObj_A5_b70* p;
    ~UnkGrappleRef_A5_b70() {
        if (p && --p->ref_count == 0)
            reinterpret_cast<void (*)(UnkGrappleRefObj_A5_b70*)>(p->vtable[0x10 / 8])(p);
    }
};

struct UnkGrappleRecord_A5_b70 {
    UnkGrappleRef_A5_b70 first;
    UnkGrappleRef_A5_b70 second;
    u8 pad_0x10[0x38];
};

struct UnkGrappleRecordList_A5_b70 {
    void** vtable;
    u32 count;
    UnkGrappleRecord_A5_b70 items[16];  // 0xc
    u8 pad_0x48c[0xc];
    u32 field_0x498;

};
#pragma pack(pop)
static_assert(sizeof(UnkGrappleRecord_A5_b70) == 0x48, "record");
static_assert(__builtin_offsetof(UnkGrappleRecordList_A5_b70, field_0x498) == 0x498, "list");

static inline __attribute__((always_inline)) void Destroy_A5_b70(UnkGrappleRecordList_A5_b70* self) {
    self->field_0x498 = 0;
    self->vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a75548) + 0x10);
    for (u64 i = 0; i < self->count; ++i)
        self->items[i].~UnkGrappleRecord_A5_b70();
    self->count = 0;
}

void fn_710081c15c(UnkGrappleRecordList_A5_b70* self) { Destroy_A5_b70(self); }

// The deleting destructor; __builtin_operator_delete lets the count store go, as in the original.
void fn_710081c770(UnkGrappleRecordList_A5_b70* self) {
    Destroy_A5_b70(self);
    __builtin_operator_delete(self);
}
