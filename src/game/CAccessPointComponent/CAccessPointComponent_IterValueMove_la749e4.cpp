// CAccessPointComponent unit (tools/tu_map.py, unit 1581): the move assignment of the iterator value over a
// keyed collection of (string id, array of string ids) pairs. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "types.h"

using base::global::CStrId;

// An array of string ids, moved by releasing its own ids and taking over the other's buffer.
struct UnkIdArray_la749e4 {
    UnkElemArray<CStrId> items;

    // Releases every id; the buffer is kept.
    void Clear() {
        for (u32 i = 0; i < items.count; ++i)
            items.data[i].Reset();
        items.count = 0;
    }

    UnkIdArray_la749e4& operator=(UnkIdArray_la749e4&& other) {
        if (this != &other) {
            Clear();
            items.allocator = other.items.allocator;
            items.flags = other.items.flags;
            items.step = other.items.step;
            items.field_0x14 = other.items.field_0x14;
            items.count = other.items.count;
            items.capacity = other.items.capacity;
            items.data = other.items.data;
            other.items.data = nullptr;
            other.items.count = 0;
            other.items.capacity = 0;
        }
        return *this;
    }
};
static_assert(sizeof(UnkIdArray_la749e4) == 0x28, "UnkIdArray_la749e4 layout");

using UnkIdArrayValue_la749e4 = base::UnkIterValue<UnkIdArray_la749e4, CStrId>;

// Moves the pair `other` holds or refers to into the one `self` holds or refers to.
UnkIdArrayValue_la749e4* fn_7100a749e4(UnkIdArrayValue_la749e4* self, UnkIdArrayValue_la749e4* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkIdArray_la749e4&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        self->Element() = static_cast<UnkIdArray_la749e4&&>(*other->ref.element);
    }
    return self;
}
