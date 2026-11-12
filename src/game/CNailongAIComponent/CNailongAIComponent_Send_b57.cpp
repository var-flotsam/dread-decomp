// The CNailongAIComponent unit. fn_7100621174 runs the CAIComponent step fn_710110cca4, then sends the
// owner's (+0x30) sub-object at +0x220 the id it holds under the usual track kind (fn_7101177ae8) again,
// under kind 0xa8067db9aac6a143 with mode 0 (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h).
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

const base::global::CStrId* fn_7101177ae8(UnkAnimationComponent* self, u64 kind);

struct UnkNailongOwner_b57a4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkNailongAI_b57a4 {
    u8 pad_0x0[0x30];
    UnkNailongOwner_b57a4* owner;
};

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
void fn_7100621174(UnkNailongAI_b57a4* self, void* arg) {
    fn_710110cca4(self, arg);
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (target) {
        base::global::UnkStringData* id = fn_7101177ae8(target, 0xf4061ff25a2163d6)->data;
        u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id) + 0x30);
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xa8067db9aac6a143, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    }
}
