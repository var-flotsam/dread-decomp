// CCharClassDissolveFanMudEvent (unit 1835 of tools/tu_map.py, the name from reflection): finds an entity by a
// fixed hash in the game manager (fn_7100debbc8, kind g_unk_71727bebb8) and, when it has its timeline (+0x2e8),
// sets a fixed timeline id on it to the all-flags mask (the hashed-at-build-time twin of
// fn_7100bcda3c, ../CCharClassRestoreFanMudEvent/). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManager.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkWeakTarget;
extern u32 g_unk_71727bebb8;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

void fn_7100bcce5c(void* self) {
    base::reflection::UnkReflTypeIdArg mask;
    u64 key;
    u64 id;
    alignas(8) u32 kind;
    key = 0x75d0d6612fb124a7;
    asm("" ::: "memory");
    id = 0x9dd5fec91d9f3e42;
    UnkGameManager* manager = g_unk_7101d47310;
    kind = g_unk_71727bebb8;
    UnkWeakTarget* entity = fn_7100debbc8(manager, &key, &kind);
    if (!entity)
        return;
    if (auto* timeline = static_cast<UnkTimelineComponent*>(reinterpret_cast<UnkActor*>(entity)->field_0x2e8)) {
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    }
}
