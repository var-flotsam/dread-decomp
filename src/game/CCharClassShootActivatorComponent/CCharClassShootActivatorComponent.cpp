// A callback list (base/UnkCloneHelpers.h's UnkCallbackList layout) with a string id at +0x40: its
// constructor (fn_7100893dc8). The base sets its vtable and an empty array of callables (step 10,
// flags 0xf), the list clears its fields, stores its own vtable and default-constructs the id.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::UnkStringData;

extern void* g_unk_7101a20998[];  // the base's vtable
extern void* g_unk_7101a33fa0[];  // the list's vtable

struct UnkCallbackIdList {
    char* vtable;
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x1c;
    u32 flags;
    base::UnkAllocator* allocator;
    u32 field_0x30;
    void* field_0x38;
    UnkStringData* id;
};

void fn_7100893dc8(UnkCallbackIdList* self) {
    __builtin_memset(&self->data, 0, 0x40);
    self->flags = 0xf;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    self->step = 10;
    self->field_0x1c = true;
    self->allocator = fn_7100083990();
    self->field_0x30 = 0;
    self->field_0x38 = nullptr;
    self->id = nullptr;
    self->vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id = &g_unk_7101d47108->empty;
}
