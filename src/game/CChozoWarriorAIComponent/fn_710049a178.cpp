// CChozoWarriorAIComponent's unit (tools/tu_map.py): when the owner's (+0x30) sub-object at +0x220 exists,
// passes a tuning float to vtable slot 0x838 and, when +0x14999 is set, sends the sub-object a zero and picks
// one of two attacks at random; then defers to fn_71011088ec. Names and layouts are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU2702.h"
#include "types.h"

extern "C" int rand();

struct UnkChozoWarriorTuning_l49a178 {
    u8 pad_0x0[0x30];
    f32 field_0x30;
};

extern UnkChozoWarriorTuning_l49a178 g_unk_71726d0e00;

struct UnkChozoWarriorOwner_l49a178 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkChozoWarriorAttack_l49a178 {
    u8 pad_0x0[0x50];
};

struct UnkChozoWarriorAI_l49a178 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkChozoWarriorOwner_l49a178* owner;  // 0x30
    u8 pad_0x38[0x14999 - 0x38];
    bool field_0x14999;
    u8 pad_0x1499a[0x14a30 - 0x1499a];
    UnkChozoWarriorAttack_l49a178 attack_0x14a30;
    u8 pad_0x14a80[0x14ae0 - 0x14a80];
    UnkChozoWarriorAttack_l49a178 attack_0x14ae0;
    UnkChozoWarriorAttack_l49a178* current;  // 0x14b30
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l49a178, current) == 0x14b30, "");

void fn_710049a178(UnkChozoWarriorAI_l49a178* self, void* arg0, void* arg1) {
    void* target = self->owner->field_0x220;
    if (target) {
        f32 value = g_unk_71726d0e00.field_0x30;
        reinterpret_cast<void (*)(UnkChozoWarriorAI_l49a178*, f32)>(self->vtable[0x838 / 8])(self, value);
        if (self->field_0x14999) {
            base::reflection::refl_EmitU32(target, 0x972422f600dd3f97, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                           &g_unk_7172769740);
            if (self->field_0x14999 && (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) > 0.5f)
                self->current = &self->attack_0x14ae0;
            else
                self->current = &self->attack_0x14a30;
        }
    }
    fn_71011088ec(self, arg0, arg1);
}
