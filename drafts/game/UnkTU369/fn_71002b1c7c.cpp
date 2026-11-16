// Unit 369: adds an object to the list of its pool entry unless it is already there, then reports it.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkPool;
struct UnkListedObject_l2b1c7c {
    u16 field_0x0;
    u16 id;  // 0x2
};
struct UnkListOwner_l2b1c7c {
    u8 pad_0x0[0x28];
    u8 listeners;  // 0x28
};

base::global::CRntVector<u64>* fn_71002b1a3c(UnkPool* pool, u32 id);
void fn_71002b1eac(void* listeners, const u64* object);

void fn_71002b1c7c(UnkListOwner_l2b1c7c* self, UnkListedObject_l2b1c7c* object) {
    if (!object)
        return;
    u64 key = (u64)object;
    base::global::CRntVector<u64>* list = fn_71002b1a3c(reinterpret_cast<UnkPool*>(self), object->id);
    for (u32 i = 0; i < list->count; ++i)
        if (list->data[i] == key)
            return;
    list->PushBack(key);
    fn_71002b1eac(&self->listeners, &key);
}
