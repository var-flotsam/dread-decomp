// Unit 424 (CPlaythrough): the destructor of the object fn_71002eba7c builds. Clears its records, then
// tears its members down in reverse order (the set at +0x110, the two owned objects, the records, the
// string ids, the id table and the resource) and runs the asset's destructor. Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkStrIdPathPair.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CPlaythrough.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkRecords_l2ef66c;
void CPlaythrough_TruncateRecords(UnkRecords_l2ef66c* self, u32 n);
void fn_71002ef8a4(base::UnkDeferredSet<void>* set, void* item);

// A string id's destructor on the given pool: decrement, then test the reloaded count.
static inline void ReleaseId(UnkStringData** id, UnkStringPool** global_pool) {
    if (UnkStringPool* pool = *global_pool) {
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                StringPool_Release(pool, *id);
                pool = *global_pool;
            }
        }
        *id = &pool->empty;
    }
}

// An object destroyed through its vtable (slot 0x10).
struct UnkOwnedObject_l2ebc18 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkOwnedPtr_l2ebc18 {
    UnkOwnedObject_l2ebc18* ptr;

    void Reset() {
        UnkOwnedObject_l2ebc18* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x10();
    }
};

// The set registration at +0x110: removed from its set when torn down.
struct UnkSetEntry_l2ebc18 {
    base::UnkDeferredSet<void>* set;
    void* field_0x8;
    void* field_0x10;

    void Clear() {
        if (set) {
            fn_71002ef8a4(set, this);
            set = nullptr;
        }
        field_0x8 = nullptr;
        field_0x10 = nullptr;
    }
};

// The records at +0xb0 (the array fn_71002ef66c shrinks).
struct UnkRecordArray_l2ebc18 {
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;

    void Truncate(u32 n) { CPlaythrough_TruncateRecords(reinterpret_cast<UnkRecords_l2ef66c*>(this), n); }
    void Free() {
        if (data) {
            Truncate(0);
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkIdTable_l2ebc18;
void fn_71002ecaec(UnkIdTable_l2ebc18* self);

// The table at +0x78: its entries are destroyed by fn_71002ecaec, then both arrays are freed.
struct UnkIdTable_l2ebc18 {
    void* data;
    void* field_0x8;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;

    void Destroy() {
        fn_71002ecaec(this);
        if (field_0x8)
            allocator->vf_0x48(field_0x8);
        if (data)
            allocator->vf_0x48(data);
    }
};

struct UnkPlaythrough_l2ebc18 {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkStringData* field_0x58;        // CStrId
    UnkStringData* field_0x60;        // CStrId
    UnkStringData* field_0x68;        // CFilePathStrId
    UnkCountedResource* resource;     // 0x70
    UnkIdTable_l2ebc18 table;         // 0x78
    u8 pad_0xa0[0x8];
    UnkStringData* field_0xa8;        // CStrId
    UnkRecordArray_l2ebc18 records;   // 0xb0
    u8 pad_0xd8[0x28];
    UnkOwnedPtr_l2ebc18 field_0x100;
    UnkOwnedPtr_l2ebc18 field_0x108;
    UnkSetEntry_l2ebc18 entry;        // 0x110
};
static_assert(sizeof(UnkPlaythrough_l2ebc18) == 0x128, "layout");

void fn_71002ebc18(UnkPlaythrough_l2ebc18* self) {
    self->vtable = g_unk_7101a22200 + 2;
    self->records.Truncate(0);
    self->entry.Clear();
    self->field_0x108.Reset();
    self->field_0x100.Reset();
    self->records.Free();
    ReleaseId(&self->field_0xa8, &g_unk_7101d47108);
    self->table.Destroy();
    fn_7100027044(reinterpret_cast<UnkStrIdPathResource*>(&self->field_0x60));
    if (UnkCountedResource* p = self->resource)
        if (--p->ref_count == 0)
            fn_710001737c(p);
    ReleaseId(&self->field_0x68, &g_unk_7101d47200);
    ReleaseId(&self->field_0x60, &g_unk_7101d47108);
    ReleaseId(&self->field_0x58, &g_unk_7101d47108);
    fn_7100016f88(self);
}
