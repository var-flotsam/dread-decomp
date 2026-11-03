// Pool component units (tools/tu_map.py: CCharClassLavaPoolComponent, and its copy in
// CCharClassWaterPoolComponent): the qualified id of the unit's blackboard property, built on first
// use (game/UnkGameBlackboard.h). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

extern UnkPropertyEx<> g_unk_7172733360;  // the lava pool's property
extern UnkPropertyEx<> g_unk_7172734660;  // the water pool's property

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (__builtin_expect((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10, 0))
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

const UnkQualifiedId& fn_71008fceb4() { return GetQualifiedId(g_unk_7172733360); }

const UnkQualifiedId& fn_710090a62c() { return GetQualifiedId(g_unk_7172734660); }
