// Unit 25: adds a key to a map of string ids to 0x80-byte records (values at 0, keys at 8, sharing one
// count; the layout of src/base/UnkAssetRecordMap.cpp's UnkIdMap): grows it by a step when full, stores a
// copy of the key and a new empty record, and returns the record. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// As src/base/core/CAsset/fn_7100018558.cpp declares them.
template <class V>
struct UnkIdMap;
struct UnkRecord80;
using UnkIdRecordMap = UnkIdMap<UnkRecord80>;
void fn_7100017948(UnkIdRecordMap* self, u32 capacity);  // Reserve

struct UnkEmptyMap_l01ab7c {
    void* values;
    void* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;

    UnkEmptyMap_l01ab7c() {
        values = nullptr;
        keys = nullptr;
        count = 0;
        capacity = 0;
        step = 10;
        flags = 0xe;
        allocator = GetDefaultAllocator();
    }
};

struct UnkNewRecord_l01ab7c {
    UnkEmptyMap_l01ab7c field_0x0;
    UnkEmptyMap_l01ab7c field_0x28;
    base::global::CRntVector<void*> field_0x50;
    base::global::CFilePathStrId field_0x78;

    UnkNewRecord_l01ab7c() : field_0x50(9) {}
};
static_assert(sizeof(UnkNewRecord_l01ab7c) == 0x80, "record layout");

using UnkRecordMap_l01ab7c = base::global::CRntSmallDictionary<base::global::CStrId, UnkNewRecord_l01ab7c>;

UnkNewRecord_l01ab7c* fn_710001ab7c(UnkRecordMap_l01ab7c* self, const base::global::CStrId* key) {
    if (self->count == self->capacity)
        fn_7100017948(reinterpret_cast<UnkIdRecordMap*>(self), self->count + self->step);
    new (&self->keys[self->count]) base::global::CStrId(*key);
    new (&self->values[self->count]) UnkNewRecord_l01ab7c();
    u32 n = ++self->count;
    UnkNewRecord_l01ab7c* end = &self->values[n];
    return n ? &end[-1] : end;
}
