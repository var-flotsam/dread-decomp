// CQuetzoaXMultiTargetAttack's unit (tools/tu_map.py): a message handler. For a message of kind 1 or 2
// whose sender's track (+0x8 -> +0x10 -> +0x38 -> +0x30) is 0x44b65b68ed9f9dae, while the owner (+0x8)
// has a character class and a +0x1b8 object not in state 2: counts the hits down (+0x38); until the
// last one the class is sent that track again with mode 2, at the last one track 0xb60395f638ac5184 with
// mode 1 (as src/game_s2/UnkSpitSend.h). Always returns true. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkQuetzoaXTrack_b61 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkQuetzoaXAnim_b61 {
    u8 pad_0x0[0x38];
    UnkQuetzoaXTrack_b61* track;
};

struct UnkQuetzoaXSender_b61 {
    u8 pad_0x0[0x10];
    UnkQuetzoaXAnim_b61* anim;
};

struct UnkQuetzoaXMessage_b61 {
    u32 kind;
    UnkQuetzoaXSender_b61* sender;
};

struct UnkQuetzoaXState_b61 {
    u8 pad_0x0[0x70];
    s32 state;
};

struct UnkQuetzoaXMultiTarget_b61 {
    void** vtable;
    UnkActor* owner;
    u8 pad_0x10[0x38 - 0x10];
    s32 hits;
};

bool fn_710063e004(UnkQuetzoaXMultiTarget_b61* self, UnkQuetzoaXMessage_b61* msg) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
start:
    void* cls = self->owner->field_0x220;
    UnkQuetzoaXState_b61* state = static_cast<UnkQuetzoaXState_b61*>(self->owner->field_0x1b8);
    if (!cls || !state)
        return true;
    if (state->state == 2)
        return true;
    if (msg->kind - 1 > 1)
        return true;
    const u64 track = 0x44b65b68ed9f9dae;
    if (msg->sender->anim->track->hash != track)
        return true;
    if (--self->hits != 0) {
        mode1.id = 2;
        mask1.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, track, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740, &mask1);
    } else {
        mode2.id = 1;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, 0xb60395f638ac5184, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                      &mask2);
    }
    return true;
}
