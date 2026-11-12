// CCharClassRestoreFanMudEvent (unit 1837 of tools/tu_map.py, the name from reflection): finds the entity
// "fan_cooldown_000" in the game manager (fn_7100debbc8, kind g_unk_71727bebb8) and, when it has its timeline
// (+0x2e8), sets "restore" on it to the all-flags mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkGameManager.h"
#include "game/UnkActor.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkWeakTarget;

void fn_7100bcda3c(void* self) {
    base::reflection::UnkReflTypeIdArg mask;
    u64 key;
    alignas(8) u32 kind;
    UnkGameManager* manager = g_unk_7101d47310;
    key = base::global::CName("fan_cooldown_000").hash;
    kind = g_unk_71727bebb8;
    UnkWeakTarget* entity = fn_7100debbc8(manager, &key, &kind);
    if (!entity)
        return;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(reinterpret_cast<UnkActor*>(entity)->field_0x2e8)) {
        key = base::global::CName("restore").hash;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
    }
}
