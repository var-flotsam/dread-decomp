// A callback list with a string id at +0x40 (the layout of fn_7100893dc8 in
// CCharClassShootActivatorComponent.cpp): this unit's own copy of its constructor.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"

using base::global::UnkStringData;

namespace {
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
}  // namespace

void fn_71008a9c2c(void* p) {
    UnkCallbackIdList* self = static_cast<UnkCallbackIdList*>(p);
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
