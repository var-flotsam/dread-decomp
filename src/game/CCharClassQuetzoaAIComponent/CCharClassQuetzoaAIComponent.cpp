// Two constructors in the CCharClassQuetzoaAIComponent unit: the boss constructor fn_710041a198,
// the vtable, the members at +0x528 and +0x618 (fn_7100642cd0, fn_7100643980), a flag, the float
// at +0x4e0 (0.8) and the two flag bytes at +0x4fc; the second then makes the post-construction
// virtual call (slot 3). Names and layouts are placeholders.
#include "game/UnkBossAIComponent.h"
#include "types.h"

void fn_7100642cd0(void* self);
void fn_7100643980(void* self);
extern void* g_unk_7101a5a128[];

struct UnkQuetzoaDef : UnkBossAIComponent {
    u8 field_0x528[0xf0];
    u8 field_0x618[0xf0];
};

__attribute__((always_inline)) static inline void ConstructQuetzoa(UnkQuetzoaDef* self) {
    fn_710041a198(self);
    self->vtable = g_unk_7101a5a128 + 2;
    fn_7100642cd0(self->field_0x528);
    fn_7100643980(self->field_0x618);
    reinterpret_cast<u8*>(self)[0x50c] = 1;
    *reinterpret_cast<float*>(reinterpret_cast<u8*>(self) + 0x4e0) = 0.8f;
    self->field_0x4fc = 0x101;
}

void fn_71006490e8(UnkQuetzoaDef* self) {
    ConstructQuetzoa(self);
}

void fn_71006493bc(UnkQuetzoaDef* self) {
    ConstructQuetzoa(self);
    reinterpret_cast<void (*)(UnkQuetzoaDef*)>(self->vtable[3])(self);
}
