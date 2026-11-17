// CChozoWarriorAIComponent unit (tools/tu_map.py): constructor of a record of three string ids from a list
// of up to three: all three start empty (creating the pool first when there is none), then each listed id
// is copied in order; a longer list leaves them empty. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

struct UnkStrIds3_l497b38 {
    CStrId ids[3];
};

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100497b38(void* self, const UnkStrIdBase* ids, u64 count) {
    UnkStrIds3_l497b38* record = static_cast<UnkStrIds3_l497b38*>(self);
    InitEmptyStrId(&record->ids[0]);
    InitEmptyStrId(&record->ids[1]);
    InitEmptyStrId(&record->ids[2]);
    if (count < 4) {
        const CStrId* list = static_cast<const CStrId*>(ids);
        u32 n = 0;
        for (const CStrId* it = list; it != list + count; ++it) {
            CStrId id = *it;
            record->ids[n++] = id;
        }
    }
}
