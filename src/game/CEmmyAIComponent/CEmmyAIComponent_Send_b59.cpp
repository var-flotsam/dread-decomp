// The CEmmyAIComponent unit. fn_71005474f4: when the owner's (+0x30) animation (+0x220) has a
// CCharClassEmmyStopAndSearchTrack, or `force` is set, sends the animation a zero under one of three ids:
// the first when the owner's tracked object (+0x218) answers fn_7100979ef0, the second when the animation
// finds g_unk_71726df420 (fn_710117af10), else the third. Descriptors as UnkSpitSend
// (src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

bool fn_7100979ef0(void* tracked);

// As in src/game/CGoliathXBurstProjectionBombsAttack/CGoliathXBurstProjectionBombsAttack_Send_b57.cpp.
struct UnkStruct_710117af10;
struct UnkCls_710117af10;
bool fn_710117af10(UnkStruct_710117af10* self, UnkCls_710117af10** name, bool skipBusy);
extern UnkCls_710117af10* g_unk_71726df420;  // reached through the GOT

struct UnkEmmySearchOwner_b59a3 {
    u8 pad_0x0[0x218];
    void* field_0x218;
    UnkAnimationComponent* field_0x220;
};

struct UnkEmmySearchAI_b59a3 {
    u8 pad_0x0[0x30];
    UnkEmmySearchOwner_b59a3* owner;
};

void fn_71005474f4(UnkEmmySearchAI_b59a3* self, bool force) {
    UnkAnimationComponent* target;
    // The gotos skip the declarations below, so they get no lifetime markers and keep slots of their own.
    if (!fn_710117b348(self->owner->field_0x220, fn_7100c18138()) && !force)
        return;
    if (fn_7100979ef0(self->owner->field_0x218))
        goto first;
    if (fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(self->owner->field_0x220), &g_unk_71726df420, false))
        goto second;
    goto third;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
    base::reflection::UnkReflTypeIdArg mode3;
    base::reflection::UnkReflTypeIdArg mask3;
first:
    target = self->owner->field_0x220;
    mode1.id = 0;
    mask1.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0xf79899dcc9c61893, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                  &mask1);
    return;
second:
    target = self->owner->field_0x220;
    mode2.id = 0;
    mask2.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0x390c7a63da50b2ac, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                  &mask2);
    return;
third:
    target = self->owner->field_0x220;
    mode3.id = 0;
    mask3.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0x64e3f7fcfd88e5c0, 0xf4061ff25a2163d6, &mode3, &g_unk_710164b958, &g_unk_7172769740,
                  &mask3);
}
