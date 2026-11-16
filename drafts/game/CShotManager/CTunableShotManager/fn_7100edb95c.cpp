// CShotManager::CTunableShotManager's unit (tools/tu_map.py, unit 2576): the copy constructor of a block of
// eight file-path ids: each id is first made empty, then a copy of the other's. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPathIds_ledb95c {
    CFilePathStrId ids[8];
};

// NON_MATCHING(major): the empty ids' pool checks fold more than the original's (it reloads the pool
// before the sixth, seventh and eighth id); the copies match.
void fn_7100edb95c(UnkPathIds_ledb95c* self, const UnkPathIds_ledb95c* other) {
    for (u32 i = 0; i < 8; ++i)
        new (&self->ids[i]) CFilePathStrId();
    for (u32 i = 0; i < 8; ++i)
        new (&self->ids[i]) CFilePathStrId(other->ids[i]);
}
