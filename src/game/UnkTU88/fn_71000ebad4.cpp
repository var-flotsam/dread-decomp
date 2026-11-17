// Unit 88 (tools/tu_map.py): the resource registry's destructor (its constructor and layout are in
// src/base/UnkResRegistry.cpp): its members are destroyed last to first: the pooled list, the path id, the two
// dictionaries, the pooled type set and the first set. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkResDesc.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "base/global/UnkBucketSet.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringPool;

struct UnkSetNode_b257j;
struct UnkFirstSetNode_l0ebad4;

using UnkNamedSet_b257j = base::global::UnkBucketSet<UnkSetNode_b257j>;
void fn_71000ea16c(UnkNamedSet_b257j* self);
void fn_71000ea064(void* list);

extern char g_unk_7101a1b888[];  // the registry's vtable
extern char g_unk_7101a1b918[];
extern char g_unk_7101a1b938[];

struct UnkResListTag_l0ebad4;
template <>
struct base::UnkPoolListTraits<UnkResListTag_l0ebad4> {
    static char* vtable() { return g_unk_7101a1b918; }
    static char* base_vtable() { return g_unk_7101a1b938; }
};

// A 0x80-byte record (base/core/CAsset.h), destroyed out of line.
struct UnkRecord80Slot_l0ebad4 {
    u8 bytes[0x80];
};

template <class V, class Destroy>
static inline void DestroyDictionary(base::global::CRntSmallDictionary<CStrId, V>* dict, Destroy destroy) {
    if (dict->keys && dict->count) {
        for (u64 i = 0; i < dict->count; ++i)
            dict->keys[i].Reset();
    }
    if (dict->values) {
        for (u32 i = 0; i < dict->count; ++i)
            destroy(&dict->values[i]);
    }
    dict->count = 0;
    if (dict->keys)
        dict->allocator->vf_0x48(dict->keys);
    if (dict->values)
        dict->allocator->vf_0x48(dict->values);
}

// Releases a path id, leaving it the pool's empty id.
static inline void ResetPath(CFilePathStrId* id) {
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                fn_71000803a4(pool, id->data);
                pool = g_unk_7101d47200;
            }
        }
        id->data = &pool->empty;
    }
}

struct UnkResRegistry_l0ebad4 {
    char* vtable;
    char* vtable_0x8;
    base::global::UnkBucketSet<UnkFirstSetNode_l0ebad4> first;                         // 0x10
    base::global::UnkPooledBucketSet<UnkSetNode_b257j> types;                          // 0x60
    base::global::CRntSmallDictionary<CStrId, UnkResDesc> descs;                       // 0xf0
    base::global::CRntSmallDictionary<CStrId, UnkRecord80Slot_l0ebad4> records;        // 0x118
    CFilePathStrId path;                                                               // 0x140
    base::UnkPoolList<UnkResListTag_l0ebad4, 8, 0x10> list;                            // 0x148
};
static_assert(__builtin_offsetof(UnkResRegistry_l0ebad4, records) == 0x118, "layout");
static_assert(__builtin_offsetof(UnkResRegistry_l0ebad4, list) == 0x148, "layout");
static_assert(sizeof(UnkResRegistry_l0ebad4) == 0x1b8, "layout");

void fn_71000ebad4(UnkResRegistry_l0ebad4* self) {
    self->vtable = g_unk_7101a1b888 + 0x10;
    self->vtable_0x8 = g_unk_7101a1b888 + 0x60;
    self->list.~UnkPoolList();
    ResetPath(&self->path);
    DestroyDictionary(&self->records, [](UnkRecord80Slot_l0ebad4* record) {
        fn_7100017ce8(reinterpret_cast<UnkRecord80*>(record));
    });
    DestroyDictionary(&self->descs, [](UnkResDesc* desc) { desc->~UnkResDesc(); });
    base::global::UnkPooledBucketSet<UnkSetNode_b257j>* types = &self->types;
    types->nodes.field_0x3c = true;
    fn_71000ea16c(types);
    types->node_allocator = nullptr;
    types->nodes.~UnkBlockPool();
    fn_71000ea16c(types);
    types->buckets.Free();
    fn_71000ea064(&self->first);
    self->first.buckets.Free();
}
