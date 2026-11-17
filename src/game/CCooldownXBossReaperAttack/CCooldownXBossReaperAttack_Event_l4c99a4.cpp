// CCooldownXBossReaperAttack's unit (tools/tu_map.py): on an event of kind 1 with a fixed id, when the owner
// (+0x8) has a CCooldownXBossAIComponent (+0x240), sends the owner's character class (+0x220) one of three
// requests by the AI's state, then drops the weak handle at +0x88 after unlinking +0x70. Returns true.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkReaperAI_l4c99a4 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1138];
    u32 field_0x1140;
};

bool fn_71004d4d98(UnkReaperAI_l4c99a4* ai);
void fn_71004d1f38(UnkReaperAI_l4c99a4* ai);

struct UnkReaperOwner_l4c99a4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    UnkReaperAI_l4c99a4* field_0x240;
};

struct UnkReaperEventId_l4c99a4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkReaperEventInner_l4c99a4 {
    u8 pad_0x0[0x38];
    UnkReaperEventId_l4c99a4* field_0x38;
};

struct UnkReaperEventData_l4c99a4 {
    u8 pad_0x0[0x10];
    UnkReaperEventInner_l4c99a4* field_0x10;
};

struct UnkReaperEvent_l4c99a4 {
    u32 kind;
    u8 pad_0x4[4];
    UnkReaperEventData_l4c99a4* field_0x8;
};

struct UnkReaperAttack_l4c99a4 {
    u8 pad_0x0[0x8];
    UnkReaperOwner_l4c99a4* owner;
    u8 pad_0x10[0x60];
    u8 field_0x70[0x18];
    UnkWeakHandle* field_0x88;
};

bool fn_71004c99a4(UnkReaperAttack_l4c99a4* self, UnkReaperEvent_l4c99a4* event) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg idle_mode;
    base::reflection::UnkReflTypeIdArg idle_mask;
    base::reflection::UnkReflTypeIdArg ready_mode;
    base::reflection::UnkReflTypeIdArg ready_mask;
    base::reflection::UnkReflTypeIdArg wait_mode;
    base::reflection::UnkReflTypeIdArg wait_mask;
start:
    if (event->kind == 1 && event->field_0x8->field_0x10->field_0x38->hash == 0xdd8311a873527a55) {
        UnkReaperAI_l4c99a4* ai = self->owner->field_0x240;
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_71004cf318())) {
            void* cls = self->owner->field_0x220;
            if (ai->field_0x1140 == 0) {
                idle_mode.id = 0;
                idle_mask.id = UnkAll_SpitSend().value;
                fn_7101176690(cls, 0x2660ef84adb182ac, 0xf4061ff25a2163d6, &idle_mode, &g_unk_710164b958,
                              &g_unk_7172769740, &idle_mask);
            } else if (fn_71004d4d98(ai)) {
                ready_mode.id = 0;
                ready_mask.id = UnkAll_SpitSend().value;
                fn_7101176690(cls, 0x5e9ac815c6e36df2, 0xf4061ff25a2163d6, &ready_mode, &g_unk_710164b958,
                              &g_unk_7172769740, &ready_mask);
                fn_71004d1f38(ai);
            } else {
                wait_mode.id = 0;
                wait_mask.id = UnkAll_SpitSend().value;
                fn_7101176690(cls, 0x58dd22244357edc7, 0xf4061ff25a2163d6, &wait_mode, &g_unk_710164b958,
                              &g_unk_7172769740, &wait_mask);
            }
            if (UnkWeakHandle* handle = self->field_0x88) {
                if (handle->target)
                    fn_710117b3f0(handle->target, self->field_0x70);
                ResetRef(&self->field_0x88);
            }
        }
    }
    return true;
}
