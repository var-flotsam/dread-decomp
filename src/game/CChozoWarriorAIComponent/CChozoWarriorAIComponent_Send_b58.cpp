// CChozoWarriorAIComponent's unit (tools/tu_map.py): when the owner's (+0x30) sub-object at +0x220 exists,
// passes the float at +0x1499c to vtable slot 0x838 and sends the sub-object a one (refl_EmitU32); then
// runs fn_7100369d80. Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "types.h"

struct UnkWarriorSendOwner_b58 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkWarriorSend_b58 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkWarriorSendOwner_b58* owner;
    u8 pad_0x38[0x1499c - 0x38];
    f32 field_0x1499c;
};

void fn_7100499e2c(UnkWarriorSend_b58* self, void* arg) {
    void* target = self->owner->field_0x220;
    if (target) {
        f32 value = self->field_0x1499c;
        reinterpret_cast<void (*)(UnkWarriorSend_b58*, f32)>(self->vtable[0x838 / 8])(self, value);
        base::reflection::refl_EmitU32(target, 0xd8aa1ffac09fb2fc, 0xf4061ff25a2163d6, 1, &g_unk_710164b958,
                                       &g_unk_7172769740);
    }
    fn_7100369d80(self, arg);
}
