// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): builds a list of up to four string ids
// from an initializer list (all four start empty; a longer list leaves them so). Names and layouts are
// placeholders.
#include <initializer_list>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdList4_l4cee70 {
    CStrId ids[4];
};

// An id that holds the pool's empty string (the pool is created first when there is none).
static inline void InitEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_71004cee70(UnkIdList4_l4cee70* self, std::initializer_list<CStrId> list) {
    for (CStrId& id : self->ids)
        InitEmpty(&id);
    u32 count = list.size();
    if (count <= sizeof(self->ids) / sizeof(self->ids[0])) {
        u32 i = 0;
        for (CStrId id : list)
            self->ids[i++] = id;
    }
}
