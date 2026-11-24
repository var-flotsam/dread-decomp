// CMinimapDef's unit (2564): the list of the nine scenario ids, a static built on first use from the
// unit's id globals and freed at exit. Names and layouts are placeholders.
#include <initializer_list>

#include "UnkClone.h"
#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2564.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

struct UnkIdRecordOwner_0xf0;
using UnkIdElem = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 8, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

void ConstructStrIdVectorFromIdArray(base::UnkStrIdVec* self, const base::global::CStrId* ids, u64 size, u32 step,
                   base::IAllocator* allocator, u32 flags);
void fn_71008600a8(base::global::CRntVector<UnkIdElem>* self);

// A string-id vector (base::UnkStrIdVec's layout) built from a list of ids and freed when destroyed.
struct UnkIdList_le68844 {
    CStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    UnkIdList_le68844(std::initializer_list<CStrId> ids) {
        ConstructStrIdVectorFromIdArray(reinterpret_cast<base::UnkStrIdVec*>(this), ids.begin(), ids.size(), 10, nullptr, 0xf);
    }
    ~UnkIdList_le68844() { fn_71008600a8(reinterpret_cast<base::global::CRntVector<UnkIdElem>*>(this)); }
};

static inline const CStrId& AsId(const UnkStrIdBase& id) { return static_cast<const CStrId&>(id); }

// The scenario ids, in the order of the unit's globals.
UnkIdList_le68844* fn_7100e68844() {
    static UnkIdList_le68844 s_ids({
        AsId(g_unk_71727c7fe8), AsId(g_unk_71727c7ff0), AsId(g_unk_71727c7ff8),
        AsId(g_unk_71727c8000), AsId(g_unk_71727c8008), AsId(g_unk_71727c8010),
        AsId(g_unk_71727c8018), AsId(g_unk_71727c8020), AsId(g_unk_71727c8028),
    });
    return &s_ids;
}
