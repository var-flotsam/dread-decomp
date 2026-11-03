// CTakumakuAIComponent's unit (tools/tu_map.py; the name from reflection): runs the shared reset
// fn_7100369650, then sets the value at +0x14748 to -1.0 and clears the pointer at +0x14750 and the
// two flags at +0x14758. Names are placeholders.
#include "types.h"

// Declared as src/game/CBehaviorTreeAIComponent/fn_7100369650.cpp defines it.
void fn_7100369650();  // declared elsewhere without its argument; called with it

struct UnkTakumakuTarget_b45a2 {
    void* ptr;
    bool field_0x8;
    bool field_0x9;
    void Clear() {
        ptr = nullptr;
        field_0x8 = false;
        field_0x9 = false;
    }
};

struct UnkTakumakuAI_b45a2 {
    u8 pad_0x0[0x14748];
    f32 field_0x14748;
    u32 pad_0x1474c;
    UnkTakumakuTarget_b45a2 target;  // 0x14750
};

void fn_71006d445c(UnkTakumakuAI_b45a2* self) {
    reinterpret_cast<void (*)(UnkTakumakuAI_b45a2*)>(fn_7100369650)(self);
    asm("" : "+r"(self));  // the original addresses the fields from self after the call
    self->field_0x14748 = -1.0f;
    UnkTakumakuTarget_b45a2* target = &self->target;
    asm("" : "+r"(target));  // addressed on its own, as in the original
    target->Clear();
}
