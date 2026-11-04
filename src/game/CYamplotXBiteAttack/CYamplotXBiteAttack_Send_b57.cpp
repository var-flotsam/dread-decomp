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

CStrId fn_71006ecce4(UnkYamplotXBite_b57a4* self);

void fn_71006ecb84(UnkYamplotXBite_b57a4* self) {
    void* target = self->owner->field_0x220;
    CStrId id = fn_71006ecce4(self);
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id.data) + 0x30);
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
