// CCharClassShakernautAIComponent's unit (tools/tu_map.py unit 1046): a constructor on the base
// fn_710041b118: the vtable, the members at +0x520 (fn_71006a2c4c) and +0x620 (fn_71006a3390), then
// the post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a62e70[];  // the component's vtable

// As src/game/CCharClassRobotAIComponent/CCharClassRobotAIComponent.cpp defines it.
void fn_710041b118(char* p);
void fn_71006a2c4c(void* self);
void fn_71006a3390(void* self);

struct UnkShakernautAI_b0012 {
    void** vtable;
    u8 pad_0x8[0x520 - 0x8];
    u8 field_0x520[0x100];
    u8 field_0x620[0x8];
};

void fn_71006acf04(UnkShakernautAI_b0012* self) {
    fn_710041b118(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a62e70 + 2;
    fn_71006a2c4c(self->field_0x520);
    fn_71006a3390(self->field_0x620);
    reinterpret_cast<void (*)(UnkShakernautAI_b0012*)>(self->vtable[3])(self);
}
