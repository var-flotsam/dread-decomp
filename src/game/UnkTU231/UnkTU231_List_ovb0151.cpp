// Unit 231: the n-th entry of a list stored in one block of memory (the head's second link is the
// first entry; each entry holds a pointer to its value). Names and layouts are placeholders.
#include "types.h"

struct UnkListEntry_b0151 {
    u64* value;  // 0x0
    UnkListEntry_b0151* next;  // 0x8
};

struct UnkListOwner_b0151 {
    u8 pad_0x0[0x10];
    UnkListEntry_b0151* head;  // 0x10
};

u64 fn_71001aaae0(UnkListOwner_b0151* self, u32 index) {
    UnkListEntry_b0151* entry = self->head->next;
    if (index != 0 && entry) {
        u32 left = index - 1;
        do {
            entry = entry->next;
            if (left == 0)
                break;
            --left;
        } while (entry);
    }
    return *entry->value;
}
