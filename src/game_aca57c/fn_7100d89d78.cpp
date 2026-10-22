// Replaces the array stored under a key in a hash map (keyed by object pointers, hashed by the
// two words at +0x30 of the object): when the key is present, its array is move-assigned from
// `value`; otherwise nothing happens.
#include "base/UnkVector.h"
#include "types.h"

struct UnkMapKey {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkMapElem40 {
    virtual ~UnkMapElem40();
    u8 unk_0x8[0x38];
};

struct UnkArrayMapNode {
    UnkMapKey* key;
    base::UnkVector<UnkMapElem40> value;
    u8 unk_0x30[0x10];
    UnkArrayMapNode* next;
};

struct UnkArrayMap {
    u8 unk_0x0[0x10];
    UnkArrayMapNode** buckets;
    u32 bucket_count;
    u32 modulus;
};

void fn_7100d89d78(UnkArrayMap* self, UnkMapKey* const* key, base::UnkVector<UnkMapElem40>&& value) {
    if (self->bucket_count == 0)
        return;
    u32 index = ((*key)->field_0x30 ^ (*key)->field_0x34) % self->modulus;
    UnkArrayMapNode** bucket = self->buckets + index;
    if (!bucket)
        return;
    for (UnkArrayMapNode* node = *bucket; node; node = node->next) {
        if (node->key == *key) {
            node->value = static_cast<base::UnkVector<UnkMapElem40>&&>(value);
            return;
        }
    }
}
