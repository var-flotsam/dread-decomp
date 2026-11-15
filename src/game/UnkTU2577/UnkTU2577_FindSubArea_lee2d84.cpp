// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee2d84: finds the
// sub-area of the array at +0x860 (0x188 bytes each) that fn_7100fad66c accepts for the id, or null.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2611.h"
#include "types.h"

using base::global::CStrId;

struct UnkSubareaIdLists;

struct UnkSubAreaEntry_lee2d84 {
    u8 pad_0x0[0x188];
};

struct UnkSubAreaMgr_lee2d84 {
    u8 pad_0x0[0x860];
    UnkSubAreaEntry_lee2d84* entries;  // 0x860
    u32 count;                         // 0x868
};

struct UnkRecB_b189;
struct UnkObjMgrB_b189;

UnkRecB_b189* fn_7100ee2d84(UnkObjMgrB_b189* manager, const CStrId* id) {
    UnkSubAreaMgr_lee2d84* self = reinterpret_cast<UnkSubAreaMgr_lee2d84*>(manager);
    UnkSubAreaEntry_lee2d84* found = self->entries;
    UnkSubAreaEntry_lee2d84* end = found + self->count;
    {
        CStrId key(*id);
        for (; found != end; ++found) {
            CStrId probe(key);
            if (fn_7100fad66c(reinterpret_cast<UnkSubareaIdLists*>(found), &probe))
                break;
        }
    }
    UnkSubAreaEntry_lee2d84* limit = self->entries + self->count;
    return limit == found ? nullptr : reinterpret_cast<UnkRecB_b189*>(found);
}
