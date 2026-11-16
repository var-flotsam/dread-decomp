// Unit 369: takes an object out of the list of its pool entry, then reports the removal.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkPool;
struct UnkListedObject_l2b1de0 {
    u16 field_0x0;
    u16 id;  // 0x2
};
struct UnkListOwner_l2b1de0 {
    u8 pad_0x0[0xa8];
    u8 listeners;  // 0xa8
};

base::global::CRntVector<u64>* fn_71002b1a3c(UnkPool* pool, u32 id);
void fn_71002b1eac(void* listeners, const u64* object);

void fn_71002b1de0(UnkListOwner_l2b1de0* self, UnkListedObject_l2b1de0* object) {
    if (!object)
        return;
    u64 key = (u64)object;
    base::global::CRntVector<u64>* list = fn_71002b1a3c(reinterpret_cast<UnkPool*>(self), object->id);
    u64* values = list->data;
    u32 n = list->count;
    u64 i = 0;
    u64 found = n;
    for (; i != n; ++i) {
        if (values[i] == key) {
            found = i;
            break;
        }
    }
    if (i == n)
        return;
    if (n > (u32)found) {
        __builtin_memcpy(&values[found], &values[found + 1], (u64)(n - found - 1) * sizeof(u64));
        --list->count;
    }
    fn_71002b1eac(&self->listeners, &key);
}
