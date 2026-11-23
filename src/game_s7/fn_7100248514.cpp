// Copies of fn_7100d89d78 (src/game_aca57c/fn_7100d89d78.cpp), which the binary
// repeats at other addresses: fn_7100248514.
// Replaces the array stored under a key in a hash map (keyed by object pointers, hashed by the
// two words at +0x30 of the object): when the key is present, its array is move-assigned from
// `value`; otherwise nothing happens.
#include "base/global/CRntVector.h"
#include "game/UnkArrayMap.h"
#include "game/UnkArrayMapNode.h"
#include "game/UnkMapElem40.h"
#include "game/UnkMapKey.h"
#include "types.h"

void fn_7100248514(UnkArrayMap* self, UnkMapKey* const* key, base::global::CRntVector<UnkMapElem40>&& value) {
    if (self->bucket_count == 0)
        return;
    u32 index = ((*key)->field_0x30 ^ (*key)->field_0x34) % self->modulus;
    UnkArrayMapNode** bucket = self->buckets + index;
    if (!bucket)
        return;
    for (UnkArrayMapNode* node = *bucket; node; node = node->next) {
        if (node->key == *key) {
            node->value = static_cast<base::global::CRntVector<UnkMapElem40>&&>(value);
            return;
        }
    }
}

