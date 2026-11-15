// Unit 2566 (minimap): finds the record chained under a key in a hash table of records and returns its part at an
// index, constructing the parts up to that index first. Names and layouts are placeholders.
#include "game/UnkTU2566.h"
#include "game/UnkTU2567.h"
#include "types.h"

struct UnkKeyHolder_le91018 {
    u8 pad_0x0[0x30];
    u32 id_lo;
    u32 id_hi;
};

struct UnkRecordPart_le91018 {
    u8 data[0x578];
};

struct UnkRecordNode_le91018 {
    void* key;
    u32 field_0x8;
    UnkRecordPart_le91018 parts[4];
    u8 pad_0x15ec[0xc];
    UnkRecordNode_le91018* next;
};

struct UnkRecordTable_le91018 {
    u8 pad_0x0[0xd64210];
    u8 buckets[8];     // 0xd64210
    u32 bucket_count;  // 0xd64218
    u32 modulus;       // 0xd6421c
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void* fn_7100e91018(UnkRecordTable_le91018* self, UnkKeyHolder_le91018** keyRef, u32 index) {
    if (self->bucket_count) {
        u32 hash = (*keyRef)->id_lo ^ (*keyRef)->id_hi;
        u32 bucket = hash % self->modulus;
        if (bucket < self->bucket_count) {
            UnkRecordNode_le91018* node = *reinterpret_cast<UnkRecordNode_le91018**>(
                fn_7100eb3814(reinterpret_cast<UnkRecordBuckets*>(self->buckets), bucket));
            for (; node; node = node->next) {
                if (node->key != *keyRef)
                    continue;
                if (index + 1 > node->field_0x8) {
                    for (u32 i = node->field_0x8; i < index + 1; ++i)
                        fn_7100eb7680(&node->parts[i]);
                    node->field_0x8 = index + 1;
                }
                return &node->parts[index];
            }
        }
    }
    return nullptr;
}
