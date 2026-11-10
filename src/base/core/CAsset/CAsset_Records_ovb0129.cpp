// Unit 24 (base::core::CAsset). fn_7100017ba0 copies a 0x80-byte record (two maps of file path ids to id
// and path pairs, copied by fn_710001810c; a list of file path ids; a file path id). fn_71000171a0 gives an
// asset its cache key (a string id and a file path id) and loads it with a request through its slot 0x20.
// fn_7100016f88 is the asset's destructor (vtable g_unk_7101a19478): the source path, the map of ids to
// records (fn_7100017ce8 destroys each), the path and the id, then the base destructor (fn_710002384c).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCache.h"
#include "base/UnkDestructors.h"
#include "base/UnkResPathVec.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using UnkAssetPathId = base::UnkStrIdMemberOf<g_unk_7101d47200>;

using base::global::CFilePathStrId;

// As src/base/UnkResRegistry.cpp defines it: the copy reserves the source's capacity (fn_7100018450)
// and appends the ids one by one.
__attribute__((always_inline)) inline UnkResPathVec::UnkResPathVec(const UnkResPathVec& other)
    : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
      flags(other.flags), allocator(other.allocator) {
    fn_7100018450(this, other.capacity);
    for (const CFilePathStrId* p = other.data, *end = other.data + other.count; p != end; ++p) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100018450(this, new_capacity);
        // Copied bitwise, then the reference taken: count is reloaded after the store.
        CFilePathStrId* slot = &data[count];
        base::global::UnkStringData* value = p->data;
        __builtin_memcpy(slot, &value, sizeof(value));
        if (&g_unk_7101d47200->empty != value)
            ++value->ref_count;
        ++count;
    }
}

// The maps of file path ids to id and path pairs (src/base/core/CAsset/UnkIdRecordMapCopy_b45.cpp copies
// them with fn_710001810c).
struct UnkPathIdPairMap_b45a1;
void fn_710001810c(UnkPathIdPairMap_b45a1* self, const UnkPathIdPairMap_b45a1* other);

// A 0x80-byte record: two maps, a list of file path ids and a file path id.
struct UnkRecord80 {
    u8 map0[0x28];
    u8 map1[0x28];
    UnkResPathVec paths;  // 0x50
    CFilePathStrId path;  // 0x78
};
static_assert(sizeof(UnkRecord80) == 0x80, "UnkRecord80 layout");

// The record's copy constructor. Declared elsewhere as returning the record; the original returns nothing.
void fn_7100017ba0(UnkRecord80* self, const UnkRecord80* other) {
    fn_710001810c(reinterpret_cast<UnkPathIdPairMap_b45a1*>(self->map0),
                  reinterpret_cast<const UnkPathIdPairMap_b45a1*>(other->map0));
    fn_710001810c(reinterpret_cast<UnkPathIdPairMap_b45a1*>(self->map1),
                  reinterpret_cast<const UnkPathIdPairMap_b45a1*>(other->map1));
    new (&self->paths) UnkResPathVec(other->paths);
    new (&self->path) CFilePathStrId(other->path);
}


// The cache key: a string id and a file path id.
struct UnkAssetKey_b0129 {
    base::UnkStrIdMember id;
    UnkAssetPathId path;
};

struct UnkAsset_b0129;
struct UnkAssetVtable_b0129 {
    void* slots_0x0[4];
    bool (*load)(UnkAsset_b0129* self, base::UnkLoadRequest* request);  // 0x20
};

struct UnkAsset_b0129 {
    UnkAssetVtable_b0129* vtable;
    base::UnkStrIdMember id;  // 0x8
    UnkAssetPathId path;      // 0x10
    u8 pad_0x18[0x28];
    UnkAssetPathId source;  // 0x40
    u64 request_word;       // 0x48
};

// Gives the asset its key and loads it with the request (its slot 0x20); with no request it only
// takes the key.
bool fn_71000171a0(void* obj, const char* key, base::UnkLoadRequest* request) {
    UnkAsset_b0129* self = static_cast<UnkAsset_b0129*>(obj);
    const UnkAssetKey_b0129* k = reinterpret_cast<const UnkAssetKey_b0129*>(key);
    self->id = k->id;
    self->path = k->path;
    if (request) {
        self->request_word = request->field_0x8;
        self->source = k->path;
        return self->vtable->load(self, request);
    }
    self->request_word = 0;
    self->source = k->path;
    return true;
}


void fn_7100017ce8(UnkRecord80* record);  // the record's destructor
void fn_710002384c(void* self);
extern char g_unk_7101a19478[];

// The map of string ids to records (values at 0, keys at 8, sharing one count).
struct UnkAssetRecordMap_b0129 {
    UnkRecord80* values;
    base::UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkAssetRecordMap_b0129() {
        if (keys)
            for (u64 i = 0; i < count; ++i)
                keys[i].~UnkStrIdMemberOf();
        if (values)
            for (u64 i = 0; i < count; ++i)
                fn_7100017ce8(&values[i]);
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// The asset: an id, a file path id, the record map and the file path id it was loaded from.
struct UnkAssetBody_b0129 {
    char* vtable;
    base::UnkStrIdMember id;           // 0x8
    UnkAssetPathId path;               // 0x10
    UnkAssetRecordMap_b0129 records;   // 0x18
    UnkAssetPathId source;             // 0x40
};
static_assert(__builtin_offsetof(UnkAssetBody_b0129, source) == 0x40, "layout");

// The asset's destructor: the members in reverse order, then the base destructor.
__attribute__((flatten)) void fn_7100016f88(void* obj) {
    UnkAssetBody_b0129* self = static_cast<UnkAssetBody_b0129*>(obj);
    self->vtable = g_unk_7101a19478 + 0x10;
    self->source.~UnkAssetPathId();
    self->records.~UnkAssetRecordMap_b0129();
    self->path.~UnkAssetPathId();
    self->id.~UnkStrIdMemberOf();
    fn_710002384c(self);
}
