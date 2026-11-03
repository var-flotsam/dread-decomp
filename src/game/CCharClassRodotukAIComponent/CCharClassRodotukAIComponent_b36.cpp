// In the CCharClassRodotukAIComponent unit: the constructor without the post-construction call (as
// fn_71006707e8, CCharClassRodotukAIComponent.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// Declared as src/game/CCharClassYamplotXAIComponent/CCharClassYamplotXAIComponent.cpp does.
struct UnkObject_7100416db8;
void fn_7100416db8(UnkObject_7100416db8* self);
void fn_7100669dec(void* self);
extern void* g_unk_7101a5db38[];

// The array at +0x518: data, count, capacity, growth, a flag, then the allocator.
struct UnkRodotukArray {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    bool field_0x14;
    u32 field_0x18;
    base::UnkAllocator* allocator;
};

struct UnkRodotukDef2 {
    void** vtable;
    u8 pad_0x8[0x510];
    UnkRodotukArray field_0x518;
    u8 field_0x540[0x10];
};

void fn_710066faa8(UnkRodotukDef2* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a5db38 + 2;
    UnkRodotukArray* array = &self->field_0x518;
    array->field_0x0 = 0;
    array->field_0x8 = 0;
    array->field_0x10 = 10;
    array->field_0x14 = true;
    array->field_0x18 = 15;
    array->allocator = fn_7100083990();
    fn_7100669dec(self->field_0x540);
}
