// Pool component units (tools/tu_map.py: CCharClassLavaPoolComponent, and its copy in
// CCharClassWaterPoolComponent): the qualified id of the unit's blackboard property, built on first
// use (game/UnkGameBlackboard.h). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CCharClassLavaPoolComponent.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (__builtin_expect(qid.id.data == &g_unk_7101d47108->empty, 0))
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

const UnkQualifiedId& CLavaPoolComponent_GetLavaVolumesId() { return GetQualifiedId(g_unk_7172733360); }

const UnkQualifiedId& CWaterPoolComponent_GetWaterVolumesId() { return GetQualifiedId(g_unk_7172734660); }
