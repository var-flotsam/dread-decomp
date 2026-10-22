// Adjusts the value at `value` by the entry for the key fn_71002874e0(a, b) in the hash table at
// +0x18 (nodes of a key and a next pointer; 0x20 buckets counted, 0x24 the modulus): when the key is
// there and the factor at +0xc is at least 1 the value is multiplied by it; either way the offset at
// +0x8 is then added. When it is missing, a factor strictly between 0 and 1 is replaced by the
// factor at +0x10 times the value. Returns the stored value (or the factor when nothing is stored).
#include "types.h"

struct UnkNode {
    u32 key;
    u32 pad_0x4;
    UnkNode* next;
};

struct UnkTable {
    u8 pad_0x0[8];
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    u8 pad_0x14[4];
    UnkNode** buckets;
    u32 count;
    u32 modulus;
};

u32 fn_71002874e0(u32 a, u32 b);

// NON_MATCHING(minor): the final add has its operands swapped (s1 + s0 in the original; same value)
f32 fn_71002b27d4(UnkTable* self, u32 a, u32 b, f32* value) {
    u32 key = fn_71002874e0(a, b);
    if (self->count != 0) {
        u32 index = key % self->modulus;
        if (index < self->count) {
            for (UnkNode* node = self->buckets[index]; node; node = node->next) {
                if (node->key == key) {
                    f32 factor = self->field_0xc;
                    f32 result = *value;
                    if (factor >= 1.0f) {
                        result = factor * result;
                        *value = result;
                    }
                    result = self->field_0x8 + result;
                    *value = result;
                    return result;
                }
            }
        }
    }
    f32 factor = self->field_0xc;
    if (factor < 1.0f && factor > 0.0f) {
        factor = self->field_0x10 * *value;
        *value = factor;
    }
    return factor;
}
