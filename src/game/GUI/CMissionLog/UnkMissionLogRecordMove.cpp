// In GUI::CMissionLog's unit: the move-assignment of a value, a string id and an array of ids,
// as fn_7100e077d0 (src/game/logic/collision/CCollider/fn_7100d3d424.cpp) but checking for
// self-assignment only before the array. Names and the layout are placeholders.
#include "base/UnkFreedVectorPair.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps.
struct UnkMissionLogStrId {
    UnkStringData* data;

    ~UnkMissionLogStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }

    // Releases the old id, then takes the other's (leaving it empty).
    __attribute__((always_inline)) void MoveAssign(UnkMissionLogStrId* other) {
        this->~UnkMissionLogStrId();
        data = other->data;
        other->data = &g_unk_7101d47108->empty;
    }
};

struct UnkMissionLogRecord {
    u32 field_0x0;
    UnkMissionLogStrId id;
    UnkMovedArray<UnkMissionLogStrId> ids;
};

UnkMissionLogRecord* fn_7100b5b31c(UnkMissionLogRecord* self, UnkMissionLogRecord* other) {
    self->field_0x0 = other->field_0x0;
    self->id.MoveAssign(&other->id);
    if (self != other)
        self->ids.MoveFrom(&other->ids);
    return self;
}
