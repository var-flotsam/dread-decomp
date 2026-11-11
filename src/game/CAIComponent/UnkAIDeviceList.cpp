// A copy of fn_7100a94384 (src/game/CThermalDeviceComponent/CThermalDeviceComponent_Ctor.cpp) in
// the CAIComponent unit: the same allocator-backed object with its own final vtable. Names and
// layouts are placeholders.
#include "game/UnkTU341.h"
#include "types.h"

#include "base/UnkVector.h"

struct UnkAIDeviceList {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    bool field_0x1c;
    u32 field_0x20;
    base::UnkAllocator* field_0x28;
    u32 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
};

void fn_710111521c(UnkAIDeviceList* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->vtable = g_unk_7101a20998 + 2;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 10;
    self->field_0x1c = true;
    self->field_0x20 = 15;
    self->field_0x28 = fn_7100083990();
    self->field_0x30 = 0;
    self->vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a20950) + 0x10);
    self->field_0x38 = 0;
}
