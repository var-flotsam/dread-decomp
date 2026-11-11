// Unit 1161 of tools/tu_map.py (no class hint): resets an object: its ids and links cleared, its
// sub-object at +0xe8 reset (fn_710074580c), and the entries of the list at +0xb8 (count at +0xc0)
// released one by one. Names and layouts are placeholders.
#include "types.h"

struct UnkEntryList_b0202;
struct UnkEntry_b0202;
void fn_710074580c(void* sub);
UnkEntry_b0202** fn_7100745c5c(UnkEntryList_b0202* list, u32 index);
void fn_7100744d90(UnkEntry_b0202* entry);

struct UnkEntryList_b0202 {
    void* data;  // 0x0
    u32 count;   // 0x8
};

struct UnkHolder_b0202 {
    u8 pad_0x0[0x8];
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    s32 field_0x20;
    bool field_0x24;
    u8 pad_0x25[0xb];
    u64 field_0x30;
    u8 pad_0x38[0x80];
    UnkEntryList_b0202 entries;  // 0xb8
    u8 pad_0xc8[0x20];
    u8 sub[1];  // 0xe8
};

void fn_7100745bdc(UnkHolder_b0202* self) {
    self->field_0x24 = false;
    self->field_0x20 = -1;
    void* sub = self->sub;
    self->field_0x18 = 0;
    self->field_0x30 = 0;
    __builtin_memset(reinterpret_cast<u8*>(self) + 0x35, 0, 8);
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    fn_710074580c(sub);
    for (u32 i = 0; i < self->entries.count; ++i)
        fn_7100744d90(*fn_7100745c5c(&self->entries, i));
    self->entries.count = 0;
}
