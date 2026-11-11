// Unit 24 (base::core::CAsset): the copy constructor of a map of string ids to 0x80-byte records
// (values at 0, keys at 8, sharing one count; the layout of fn_7100018558's map in
// src/base/core/CAsset/fn_7100018558.cpp) followed by a file path id at 0x28. The storage is
// reserved (fn_7100017948), the records copied one by one (fn_7100017ba0), then the keys and the
// path copied, counting a use of each. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// As src/base/core/CAsset/fn_7100018558.cpp declares them.
template <class V>
struct UnkIdMap;
struct UnkRecord80;
using UnkIdRecordMap = UnkIdMap<UnkRecord80>;
void fn_7100017948(UnkIdRecordMap* self, u32 capacity);  // Reserve

struct UnkRecord80Bytes_b45a1 {
    u64 words[0x80 / 8];
};

struct UnkIdRecordMapWithPath_b45a1 {
    UnkRecord80Bytes_b45a1* values;
    UnkStringData** keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
    UnkStringData* path;  // 0x28, a file path id
};

void fn_7100016cfc(UnkIdRecordMapWithPath_b45a1* self, const UnkIdRecordMapWithPath_b45a1* other) {
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->values = nullptr;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100017948(reinterpret_cast<UnkIdRecordMap*>(self), other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        fn_7100017ba0(reinterpret_cast<UnkRecord80*>(&self->values[i]),
                      reinterpret_cast<const UnkRecord80*>(&other->values[i]));
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkStringData* data = other->keys[i];
        __builtin_memcpy(&self->keys[i], &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkStringData* path = other->path;
    self->path = path;
    if (&g_unk_7101d47200->empty != path)
        ++path->ref_count;
}

// The copy constructor of a map of file path ids to id and path pairs (values at 0, keys at 8):
// reserved by fn_7100018244, then the values (a string id and a file path id each) and the keys
// copied, counting a use of each.
struct UnkIdPathValue_b45a1 {
    UnkStringData* id;
    UnkStringData* path;
};

struct UnkPathIdPairMap_b45a1 {
    UnkIdPathValue_b45a1* values;
    UnkStringData** keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100018244(UnkPathIdPairMap_b45a1* self, u32 capacity);  // Reserve

void fn_710001810c(UnkPathIdPairMap_b45a1* self, const UnkPathIdPairMap_b45a1* other) {
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->values = nullptr;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100018244(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkIdPathValue_b45a1* dst = &self->values[i];
        const UnkIdPathValue_b45a1* src = &other->values[i];
        UnkStringData* id = src->id;
        __builtin_memcpy(&dst->id, &id, sizeof(id));
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        UnkStringData* path = src->path;
        __builtin_memcpy(&dst->path, &path, sizeof(path));
        if (&g_unk_7101d47200->empty != path)
            ++path->ref_count;
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkStringData* data = other->keys[i];
        __builtin_memcpy(&self->keys[i], &data, sizeof(data));
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
}

// As src/base/core/CAsset/fn_7100018558.cpp declares it: the map's copy-assignment.
UnkIdRecordMap* fn_7100018558(UnkIdRecordMap* self, const UnkIdRecordMap* other);

// An object holding an id, a file path id, a map of ids to records with a file path id (the type
// fn_7100016cfc copies) and an owned pointer, with a flag and a word at its end.
struct UnkAssetRecordHolder_b45a1 {
    void* vtable;
    base::UnkStrIdMember id;                                // 0x8
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;          // 0x10
    UnkIdRecordMapWithPath_b45a1 records;                   // 0x18, its path at 0x40
    void* owned;                                            // 0x48
    bool flag;                                              // 0x50
    u32 word;                                               // 0x54
};
static_assert(__builtin_offsetof(UnkAssetRecordHolder_b45a1, owned) == 0x48, "UnkAssetRecordHolder_b45a1 layout");

// Its move-assignment: the flag, the word and the pointer are taken (the other's cleared), the
// ids and the map copied.
UnkAssetRecordHolder_b45a1* fn_7100016e14(UnkAssetRecordHolder_b45a1* self, UnkAssetRecordHolder_b45a1* other) {
    self->word = other->word;
    self->flag = other->flag;
    other->word = 0;
    other->flag = false;
    self->id = other->id;
    self->path = other->path;
    fn_7100018558(reinterpret_cast<UnkIdRecordMap*>(&self->records),
                  reinterpret_cast<const UnkIdRecordMap*>(&other->records));
    reinterpret_cast<base::UnkStrIdMemberOf<g_unk_7101d47200>&>(self->records.path) =
        reinterpret_cast<const base::UnkStrIdMemberOf<g_unk_7101d47200>&>(other->records.path);
    self->owned = other->owned;
    other->owned = nullptr;
    return self;
}
