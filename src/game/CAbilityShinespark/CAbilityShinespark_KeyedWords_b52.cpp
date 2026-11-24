// Copy of fn_7100dd0b9c (src/game/CBossRushManager_KeyedWords_b49.cpp) in CAbilityShinespark's unit:
// appends a u32 value under a u32 key to a keyed array, growing both arrays by the step when full.
// Names and layouts are placeholders.
#include "base/global/CRntSmallDictionary.h"
#include "types.h"

using UnkKeyedWords_b52 = base::global::CRntSmallDictionary<u32, u32>;

u32* CAbilityShinespark_AppendU32DictionaryEntry(UnkKeyedWords_b52* self, const u32* key, const u32* value) {
    if (self->count == self->capacity)
        self->Grow(self->step + self->count);
    u32* key_slot = &self->keys[self->count];
    *key_slot = *key;
    // Copied as a block (the original reloads the array after it).
    __builtin_memcpy(&self->values[self->count], value, sizeof(u32));
    u32 count = self->count + 1;
    u32* data = self->values;
    self->count = count;
    // The last value (the first when the count wrapped to zero).
    return count ? data + count - 1 : data;
}
