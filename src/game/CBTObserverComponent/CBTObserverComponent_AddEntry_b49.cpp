// CBTObserverComponent's unit (tools/tu_map.py unit 2216): appends an entry under a u32 key to the
// keyed array of 0x80-byte entries (entries at +0x0, keys at +0x8, count +0x10, capacity +0x14, step
// +0x18), growing it by the step when full (fn_7100ca9158), and returns the new entry: three empty
// arrays (base::UnkVector, allocation flags 0xf) and a cleared flag. Names and layouts are placeholders.
#include <new>

#include "base/UnkPairArrays.h"
#include "base/UnkVector.h"
#include "types.h"

void fn_7100ca9158(base::UnkPairArrays* pairs, u32 capacity);

struct UnkBTObserverEntry {
    base::UnkVector<void*> field_0x0{0xf};
    base::UnkVector<void*> field_0x28{0xf};
    base::UnkVector<void*> field_0x50{0xf};
    bool field_0x78 = false;
};
static_assert(sizeof(UnkBTObserverEntry) == 0x80, "UnkBTObserverEntry layout");

struct UnkBTObserverEntries {
    UnkBTObserverEntry* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

UnkBTObserverEntry* fn_7100ca7b08(UnkBTObserverEntries* self, const u32* key) {
    if (self->count == self->capacity)
        fn_7100ca9158(reinterpret_cast<base::UnkPairArrays*>(self), self->step + self->count);
    u32* slot = &self->keys[self->count];
    *slot = *key;
    new (&self->values[self->count]) UnkBTObserverEntry();
    u32 count = self->count + 1;
    UnkBTObserverEntry* data = self->values;
    self->count = count;
    // The last entry (the first when the count wrapped to zero).
    return count ? data + count - 1 : data;
}
