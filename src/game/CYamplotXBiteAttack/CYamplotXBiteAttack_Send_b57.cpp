// The CYamplotXBiteAttack unit. fn_71006ecb84 sends the owner's (+0x8) sub-object at +0x220 the hash of
// the string id fn_71006ecce4 returns for the attack, under the usual track kind with mode 2
// (UnkSpitSend's descriptors, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkYamplotXBiteOwner_b57a4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkYamplotXBite_b57a4 {
    u8 pad_0x0[0x8];
    UnkYamplotXBiteOwner_b57a4* owner;
};

CStrId CYamplotXBiteAttack_GetDistanceSuffixedName(UnkYamplotXBite_b57a4* self);

void fn_71006ecb84(UnkYamplotXBite_b57a4* self) {
    void* target = self->owner->field_0x220;
    CStrId id = CYamplotXBiteAttack_GetDistanceSuffixedName(self);
    u64 hash = id.data->Key();
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
