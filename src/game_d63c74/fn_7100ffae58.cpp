// Move-assignment of a file-path id with three values: destroys the old id, then moves the other's
// over (the other is left with the empty id). The same as fn_7100ead030
// (src/game/logic/collision/CCollider/fn_7100d3d424.cpp) on CFilePathStrId's pool.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A file-path id whose destructor decrements and tests in two steps.
struct UnkMovedFilePathId {
    UnkStringData* data;

    // The pointer is stored bytewise (char alias info), so the destructor of the moved-from id
    // reloads the pool.
    UnkMovedFilePathId(UnkMovedFilePathId&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkMovedFilePathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkFilePathIdValues {
    UnkMovedFilePathId id;
    struct Values {
        u32 v[3];
    };

    Values values;

    UnkFilePathIdValues(UnkFilePathIdValues&& other)
        : id(static_cast<UnkMovedFilePathId&&>(other.id)), values(other.values) {}
};

UnkFilePathIdValues* fn_7100ffae58(UnkFilePathIdValues* self, UnkFilePathIdValues* other) {
    if (self != other) {
        self->~UnkFilePathIdValues();
        new (self) UnkFilePathIdValues(static_cast<UnkFilePathIdValues&&>(*other));
    }
    return self;
}
