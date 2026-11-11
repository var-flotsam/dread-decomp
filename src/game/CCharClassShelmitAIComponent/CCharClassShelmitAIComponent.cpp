// A constructor in the CCharClassShelmitAIComponent unit (fn_71006b94c8): the base constructor
// fn_7100416db8, the vtable, the member at +0x510 (fn_71006b2d74), eleven floats from +0x618 set to
// -1, the member at +0x648 (fn_71007c7ddc), then the post-construction virtual call (slot 3). Names
// and layouts are placeholders.
#include "game/CCharClassBehaviorTreeAIComponent.h"
#include "game/CCharClassShelmitAIComponent.h"
#include "game/SDropProbabilities.h"
#include "types.h"

// Declared as src/game/CCharClassGooplotAIComponent/CCharClassGooplotAIComponent.cpp does.
struct UnkObject_7100416db8;
void fn_71006b2d74(void* self);

struct UnkShelmitDef {
    void** vtable;
    u8 pad_0x8[0x508];
    u8 field_0x510[0x108];
    f32 field_0x618[11];
    u8 pad_0x644[4];
    u8 field_0x648[8];
};

void fn_71006b94c8(UnkShelmitDef* self) {
    fn_7100416db8(reinterpret_cast<UnkObject_7100416db8*>(self));
    self->vtable = g_unk_7101a63ad0 + 2;
    fn_71006b2d74(self->field_0x510);
    for (u32 i = 0; i < 11; ++i)
        self->field_0x618[i] = -1.0f;
    fn_71007c7ddc(self->field_0x648);
    reinterpret_cast<void (*)(UnkShelmitDef*)>(self->vtable[3])(self);
}
