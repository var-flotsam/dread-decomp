// CCooldownXBossWeakPointLifeComponent's unit (tools/tu_map.py): runs fn_71008a3c48 and, when its result
// is 0 or 1, takes the global instance (0x108 bytes, created on first use as
// src/game/CBehaviorTreeAIComponent/fn_7100369650.cpp) and, through its +0x100 object's +0x10 actor's
// timeline (+0x2e8), sets the id at +0x280 to the all-flags mask. Returns the result.
// Names are placeholders.
#include "base/UnkVector.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkWeakPointLife_b60a2 {
    u8 pad_0x0[0x280];
    u64 field_0x280;
};

u32 fn_71008a3c48(UnkWeakPointLife_b60a2* self, void* arg);
extern char* g_unk_71726b2398;       // the instance, reached through the GOT

struct UnkWeakPointLink_b60a2 {
    u8 pad_0x0[0x10];
    UnkActor* field_0x10;
};

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
u32 fn_71004d685c(UnkWeakPointLife_b60a2* self, void* arg) {
    u32 result = fn_71008a3c48(self, arg);
    if (result <= 1) {
        char* instance = g_unk_71726b2398;
        if (!instance) {
            instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
            fn_71011a664c(instance);
            g_unk_71726b2398 = instance;
        }
        if (auto* link = *reinterpret_cast<UnkWeakPointLink_b60a2**>(instance + 0x100)) {
            if (UnkActor* actor = link->field_0x10) {
                if (auto* timeline = static_cast<UnkTimelineComponent*>(actor->field_0x2e8)) {
                    base::reflection::UnkReflTypeIdArg mask;
                    const u64* key = &self->field_0x280;
                    asm("" : "+r"(key));
                    mask.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
                }
            }
        }
    }
    return result;
}
