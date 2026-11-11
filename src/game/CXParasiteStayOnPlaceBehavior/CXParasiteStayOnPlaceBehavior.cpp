// CXParasiteStayOnPlaceBehavior: its constructor (fn_71007cdebc). Stores the vtable and builds the
// callback list at +0x20 (base vtable with an empty array of callables: step 10, flags 0xf; then the
// list's vtable and its cleared fields). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CXParasiteStayOnPlaceBehavior.h"
#include "game/UnkTU341.h"

struct UnkCallbackListRaw {
    char* vtable;
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
    u32 field_0x30;
    u64 field_0x38;
};

struct UnkXParasiteStayOnPlaceBehavior {
    char* vtable;
    u8 pad_0x8[0x18];
    UnkCallbackListRaw field_0x20;
};

void fn_71007cdebc(UnkXParasiteStayOnPlaceBehavior* self) {
    self->vtable = g_unk_7101a71848 + 0x10;
    UnkCallbackListRaw* list = &self->field_0x20;
    list->vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    list->data = nullptr;
    list->count = 0;
    list->capacity = 0;
    list->step = 10;
    list->field_0x14 = true;
    list->flags = 0xf;
    list->allocator = fn_7100083990();
    list->field_0x30 = 0;
    list->vtable = reinterpret_cast<char*>(g_unk_7101a20950) + 0x10;
    list->field_0x38 = 0;
}
