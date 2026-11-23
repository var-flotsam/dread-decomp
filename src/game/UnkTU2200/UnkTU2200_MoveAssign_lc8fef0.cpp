// CCutSceneDef's unit (2200): the move assignment of an iterator value over records of two string ids and a
// value keyed by string ids (no self-check). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Two string ids and a value.
struct UnkIdPairRecord_lc8fef0 {
    CStrId first;
    CStrId second;
    u32 field_0x10;

    // The second id's move: it is released, then takes the other's; the other gets the empty id of the
    // pool loaded before the release.
    void TakeSecond(CStrId& other) {
        base::global::UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (second.data && second.data != &pool->empty) {
                --second.data->ref_count;
                if (second.data->ref_count == 0) {
                    StringPool_Release(pool, second.data);
                    pool = g_unk_7101d47108;
                }
            }
            second.data = &pool->empty;
        }
        second.data = other.data;
        other.data = &pool->empty;
    }
};
static_assert(sizeof(UnkIdPairRecord_lc8fef0) == 0x18, "UnkIdPairRecord_lc8fef0 layout");

typedef base::UnkIterValue<UnkIdPairRecord_lc8fef0, CStrId> UnkIdPairIterValue_lc8fef0;

// Moves the (id, record) pair `other` holds or refers to into the one `self` holds or refers to.
UnkIdPairIterValue_lc8fef0* fn_7100c8fef0(UnkIdPairIterValue_lc8fef0* self, UnkIdPairIterValue_lc8fef0* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        UnkIdPairRecord_lc8fef0& element = self->Element();
        element.first.ReleaseAndTake(other->value.element.first);
        element.TakeSecond(other->value.element.second);
        element.field_0x10 = other->value.element.field_0x10;
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        UnkIdPairRecord_lc8fef0& from = *other->ref.element;
        UnkIdPairRecord_lc8fef0& element = self->Element();
        element.first.ReleaseAndTake(from.first);
        element.TakeSecond(from.second);
        element.field_0x10 = from.field_0x10;
    }
    return self;
}
