// The CMinimapData unit (unit 2563): moves the (key, entry) pair an iterator value holds or refers to into
// another, for a keyed collection of entries with six words, a string id and a half-word. Names and layouts are
// placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;


// An entry with six words, a string id and a half-word.
struct UnkWordsIdEntry_le5ae40 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    CStrId id;
    u16 field_0x20;

    UnkWordsIdEntry_le5ae40& operator=(UnkWordsIdEntry_le5ae40&& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        // Releases the id, then takes the other's; the other gets the empty id of the pool loaded here.
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (id.data && id.data != &pool->empty) {
                --id.data->ref_count;
                if (id.data->ref_count == 0) {
                    StringPool_Release(pool, id.data);
                    pool = g_unk_7101d47108;
                }
            }
            id.data = &pool->empty;
        }
        id.data = other.id.data;
        other.id.data = &pool->empty;
        field_0x20 = other.field_0x20;
        return *this;
    }
};
static_assert(sizeof(UnkWordsIdEntry_le5ae40) == 0x28, "UnkWordsIdEntry_le5ae40 layout");

using UnkWordsIdValue_le5ae40 = base::UnkIterValue<UnkWordsIdEntry_le5ae40, CStrId>;


// Moves the pair `other` holds or refers to into the one `self` holds or refers to.
UnkWordsIdValue_le5ae40* fn_7100e5ae40(UnkWordsIdValue_le5ae40* self, UnkWordsIdValue_le5ae40* other) {
    if (other->state == 1) {
        self->Index().ReleaseAndTakeReloading(other->value.index);
        self->Element() = static_cast<UnkWordsIdEntry_le5ae40&&>(other->value.element);
    } else if (other->state == 2) {
        CStrId& key = *const_cast<CStrId*>(other->ref.index);
        self->Index().ReleaseAndTakeReloading(key);
        UnkWordsIdEntry_le5ae40& src = *other->ref.element;
        self->Element() = static_cast<UnkWordsIdEntry_le5ae40&&>(src);
    }
    return self;
}
