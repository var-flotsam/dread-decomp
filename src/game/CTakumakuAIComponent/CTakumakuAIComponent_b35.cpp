// A function in the CTakumakuAIComponent unit: on event 6, when the object at +0x30 has a component at
// +0x220 for which fn_7101178e6c(…, 11) holds, fn_71006d5bf8 runs. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"

struct UnkTakumakuComponent;
struct UnkTakumakuOwner {
    u8 unk_0x0[0x220];
    UnkTakumakuComponent* field_0x220;
};
struct UnkTakumakuAI {
    u8 unk_0x0[0x30];
    UnkTakumakuOwner* owner;
};

void fn_71006d5bf8(UnkTakumakuAI* self);

void fn_71006d5ba8(UnkTakumakuAI* self, u32 event) {
    UnkTakumakuOwner* owner = self->owner;
    if (owner == nullptr)
        return;
    asm volatile("");  // keeps the owner test ahead of the event test
    if (event != 6)
        return;
    UnkTakumakuComponent* component = owner->field_0x220;
    if (component != nullptr && fn_7101178e6c(component, 11))
        fn_71006d5bf8(self);
}
