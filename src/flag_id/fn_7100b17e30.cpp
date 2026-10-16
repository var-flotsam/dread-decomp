// Looks up (or creates) the slot for a key and stores a value in it.
#include "types.h"

u64* fn_7100b172f8(void* self, u32 key);

u64* fn_7100b17e30(void* self, const u32* key, const u64* value) {
    u64* slot = fn_7100b172f8(self, *key);
    *slot = *value;
    return slot;
}
