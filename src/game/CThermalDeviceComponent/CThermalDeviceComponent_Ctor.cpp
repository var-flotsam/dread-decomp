// In the CThermalDeviceComponent unit: fn_7100a94384 constructs an allocator-backed object. All
// 0x48 bytes are cleared first; then the base vtable g_unk_7101a20998, two cleared words, three small
// defaults and the allocator from fn_7100083990, then its own vtable g_unk_7101a33fa0, a cleared word
// and a cleared pointer. The same object is built inline at +0x80 of CEmmyWakeUpComponent
// (fn_71007d7724). Names and layouts are placeholders.
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

#include "base/UnkVector.h"

struct UnkThermalDeviceList {
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

void fn_7100a94384(UnkThermalDeviceList* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->vtable = g_unk_7101a20998 + 2;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 10;
    self->field_0x1c = true;
    self->field_0x20 = 15;
    self->field_0x28 = fn_7100083990();
    self->field_0x30 = 0;
    self->vtable = g_unk_7101a33fa0 + 2;
    self->field_0x38 = 0;
}
