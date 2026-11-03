// CSaveStationUsableComponent unit (tools/tu_map.py): the constructor on the usable base
// (fn_7100a9e244): an empty handle at +0x540 (null, id -1), cleared flags and words, 0.5 and 1.0
// settings, then fn_7100023854 on the object. Names and layouts are placeholders.
#include "types.h"

extern char g_unk_7101a92300[];  // the component's vtable

void fn_7100a9e244(void* self);  // the base constructor
struct UnkNode;
void fn_7100023854(UnkNode*);

struct UnkSaveStationUsable_b49a4 {
    char* vtable;
    u8 pad_0x8[0x538];
    void* field_0x540;
    s64 field_0x548;
    bool field_0x550;
    u8 pad_0x551[3];
    u32 field_0x554[2];
    f32 field_0x55c;
    bool field_0x560;
    u8 pad_0x561[3];
    f32 field_0x564;
    bool field_0x568;
};

void fn_7100a8acb8(UnkSaveStationUsable_b49a4* self) {
    fn_7100a9e244(self);
    self->vtable = g_unk_7101a92300 + 0x10;
    self->field_0x540 = nullptr;
    self->field_0x548 = -1;
    self->field_0x550 = false;
    __builtin_memset(self->field_0x554, 0, sizeof(self->field_0x554));
    self->field_0x55c = 0.5f;
    self->field_0x560 = true;
    self->field_0x564 = 1.0f;
    self->field_0x568 = false;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
