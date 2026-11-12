// CSlidleSpawnPointComponent unit (tools/tu_map.py): toggles the owner's (+0x30) sub-object at +0x220
// between two tracks. The id it holds under the usual track kind (fn_7101177ae8 returns a pointer to it)
// picks the other hash; unless the sub-object is in a CCharClassSlidleOutSPAgitateTrack track
// (fn_710117b348), it is sent that hash with mode 2 (as UnkSpitSend, src/game_s2/UnkSpitSend.h, with the
// mode as a parameter). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

const base::global::CStrId* fn_7101177ae8(UnkAnimationComponent* self, u64 kind);

struct UnkSlidleSendOwner_b56 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkSlidleSend_b56 {
    u8 pad_0x0[0x30];
    UnkSlidleSendOwner_b56* owner;
};

void fn_7100a1d23c(UnkSlidleSend_b56* self) {
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (target) {
        const u64 kind = 0xf4061ff25a2163d6;
        base::global::UnkStringData* id = fn_7101177ae8(target, kind)->data;
        u64 current = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id) + 0x30);
        u64 hash = current == 0xcfae220bec701cde ? 0x880458e24797e3ea : 0xcfae220bec701cde;
        asm volatile("" : "+r"(hash));
        if (!fn_710117b348(target, fn_7100c4f408())) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 2;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        }
    }
}
