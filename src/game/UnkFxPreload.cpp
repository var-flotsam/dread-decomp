// From translation unit 2739 of data/tu_map.csv (0x71011a7f34-0x71011abbd0, no class hint; its
// manager of debug-overlay models is src/game/UnkModelManager.cpp): fn_71011a9e50 registers six
// common particle effects (.bcptl) with an effect cache, each by its file path id and a kind.
// Names are placeholders.
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkFxCache;

// Registers the effect at `path` with the cache.
void fn_7100851bc0(UnkFxCache* cache, const CFilePathStrId* path, u32 kind, bool flag);

static inline void UnkPreloadFx(UnkFxCache* cache, const char* path, u32 kind) {
    CFilePathStrId id(path, true);
    fn_7100851bc0(cache, &id, kind, false);
}

void fn_71011a9e50(UnkFxCache* cache) {
    UnkPreloadFx(cache, "system/fx/generic/block_sync.bcptl", 1);
    UnkPreloadFx(cache, "system/fx/hits/melee.bcptl", 1);
    UnkPreloadFx(cache, "system/fx/generic/enemydeath.bcptl", 5);
    UnkPreloadFx(cache, "actors/characters/samus/fx/groundlanding.bcptl", 2);
    UnkPreloadFx(cache, "actors/characters/samus/fx/groundlandingrun.bcptl", 2);
    UnkPreloadFx(cache, "system/fx/generic/energyblock.bcptl", 3);
}
