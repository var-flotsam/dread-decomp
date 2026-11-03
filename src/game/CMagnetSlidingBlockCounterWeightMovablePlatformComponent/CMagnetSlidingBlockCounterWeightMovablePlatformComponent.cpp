// Constructor (copy of fn_710077e428 in CProtoCentralUnitComponent.cpp): base constructor, vtable, the
// container at +0x258 (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator),
// then the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_710091aae0(void* self);
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];
extern void* g_unk_7101a81ff8[];

struct UnkMagnetSlidingBlockCounterWeightMovablePlatformComponent_a710 {
    void** vtable;
    u8 pad_0x8[0x258 - 8];
    void** field_0x258;
    u64 field_0x260;
    u64 field_0x268;
    u32 field_0x270;
    bool field_0x274;
    u32 field_0x278;
    base::UnkAllocator* field_0x280;
    u32 field_0x288;
    u64 field_0x290;
};

void fn_710091a710(UnkMagnetSlidingBlockCounterWeightMovablePlatformComponent_a710* self) {
    fn_710091aae0(self);
    self->vtable = g_unk_7101a81ff8 + 2;
    self->field_0x258 = g_unk_7101a20998 + 2;
    self->field_0x260 = 0;
    self->field_0x268 = 0;
    self->field_0x270 = 10;
    self->field_0x274 = true;
    self->field_0x278 = 0xf;
    self->field_0x280 = fn_7100083990();
    self->field_0x288 = 0;
    self->field_0x258 = g_unk_7101a33fa0 + 2;
    self->field_0x290 = 0;
    reinterpret_cast<void (*)(UnkMagnetSlidingBlockCounterWeightMovablePlatformComponent_a710*)>(self->vtable[3])(self);
}
