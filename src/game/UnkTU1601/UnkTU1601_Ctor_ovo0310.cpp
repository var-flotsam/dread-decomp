// CThermalDeviceComponent's unit (unit 1601): the component's constructor. The base constructor runs, the
// device list (a base::UnkVector written as flat members) is emptied with step 10 and flags 0xf, the five
// ids start empty, and the component's vf_0x18 finishes. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkCallbackSetElem;

extern void* g_unk_7101a92ed0[];
void fn_7100a9e244(void*);

struct UnkThermalComp_b0237 {
    void** vtable;
    u8 pad_0x8[0x540];
    UnkCallbackSetElem* devices_data;  // 0x548
    u32 devices_count;
    u32 devices_capacity;
    u32 devices_step;
    bool devices_grow;
    u32 devices_flags;
    base::UnkAllocator* devices_allocator;
    CStrId id_0x570;
    CStrId id_0x578;
    CStrId id_0x580;
    CStrId id_0x588;
    CStrId id_0x590;
    u32 field_0x598;
    u8 pad_0x59c[1];
    u8 field_0x59d;
};
static_assert(__builtin_offsetof(UnkThermalComp_b0237, id_0x570) == 0x570, "UnkThermalComp_b0237 layout");

void fn_7100a946ec(UnkThermalComp_b0237* self) {
    fn_7100a9e244(self);
    self->devices_data = nullptr;
    self->devices_step = 10;
    self->devices_count = 0;
    self->devices_capacity = 0;
    self->vtable = g_unk_7101a92ed0 + 2;
    self->devices_grow = true;
    self->devices_flags = 15;
    self->devices_allocator = fn_7100083990();
    new (&self->id_0x570) CStrId(g_unk_7101d470e0);
    new (&self->id_0x578) CStrId(g_unk_7101d470e0);
    new (&self->id_0x580) CStrId(g_unk_7101d470e0);
    new (&self->id_0x588) CStrId(g_unk_7101d470e0);
    new (&self->id_0x590) CStrId(g_unk_7101d470e0);
    self->field_0x598 = 0;
    self->field_0x59d = 0;
    reinterpret_cast<void (*)(UnkThermalComp_b0237*)>(self->vtable[3])(self);
}
