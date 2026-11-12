// CCooldownXBossWindTunnelAttack's unit (tools/tu_map.py): when the owner's (+0x8) timeline (+0x2e8) has
// an entry for a fixed id (g_unk_7101649b38), sets it to the all-bits mask; then looks up the entity named
// by the string id g_unk_71726d8110 (fn_7100debbc8 of the game manager, with the kind g_unk_71727bebb8)
// and sends its character class (+0x220) a request (descriptors as UnkSpitSend). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CCooldownXBossWindTunnelAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkWeakTarget;
extern const u64 g_unk_7101649b38;  // reached through the GOT

struct UnkWindTunnelOwner_b59 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkWindTunnel_b59 {
    u8 pad_0x0[0x8];
    UnkWindTunnelOwner_b59* owner;
};

void fn_71004cc644(UnkWindTunnel_b59* self) {
    if (UnkTimelineComponent* timeline = self->owner->field_0x2e8) {
        if (fn_7100a2c13c(timeline, &g_unk_7101649b38)) {
            base::reflection::UnkReflTypeIdArg all;
            all.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_7101649b38, &all.id, false, -1.0f);
        }
    }
    UnkGameManager* manager = g_unk_7101d47310;
    u64 key;
    alignas(8) u32 kind;
    u32 kind_value = g_unk_71727bebb8;
    key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(g_unk_71726d8110.data) + 0x30);
    kind = kind_value;
    UnkWeakTarget* entity = fn_7100debbc8(manager, &key, &kind);
    if (!entity)
        return;
    void* anim = *reinterpret_cast<void**>(reinterpret_cast<char*>(entity) + 0x220);
    if (!anim)
        return;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(anim, 0x17b48319c6b12217, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
