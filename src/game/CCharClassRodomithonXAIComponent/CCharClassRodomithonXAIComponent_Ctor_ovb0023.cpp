// CCharClassRodomithonXAIComponent's unit (tools/tu_map.py): a definition constructor: the base part
// (fn_710066faa8), the vtable g_unk_7101a5d0b8, two -1 values, an empty array with growth step 10 and
// flags 0xf on the default allocator, the list at +0x660 (fn_7100664224), then its own vf_0x18.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkRodotukDef2;
void fn_710066faa8(UnkRodotukDef2* self);
struct UnkRodomithonList_ovb0023;
void fn_7100664224(UnkRodomithonList_ovb0023* self);

extern void* g_unk_7101a5d0b8[];

struct UnkRodomithonDef_ovb0023 {
    void** vtable;
    u8 pad_0x8[0x630 - 0x8];
    f32 field_0x630;
    f32 field_0x634;
    u64 field_0x638;
    u64 field_0x640;
    u32 field_0x648;
    bool field_0x64c;
    u32 field_0x650;
    base::UnkAllocator* allocator;  // 0x658
    u8 list[0x8];                   // 0x660
};

void fn_710066775c(UnkRodomithonDef_ovb0023* self) {
    fn_710066faa8(reinterpret_cast<UnkRodotukDef2*>(self));
    self->vtable = &g_unk_7101a5d0b8[2];
    self->field_0x630 = -1.0f;
    self->field_0x634 = -1.0f;
    self->field_0x638 = 0;
    self->field_0x640 = 0;
    self->field_0x648 = 10;
    self->field_0x64c = true;
    self->field_0x650 = 0xf;
    self->allocator = fn_7100083990();
    fn_7100664224(reinterpret_cast<UnkRodomithonList_ovb0023*>(self->list));
    reinterpret_cast<void (*)(UnkRodomithonDef_ovb0023*)>(self->vtable[3])(self);
}
