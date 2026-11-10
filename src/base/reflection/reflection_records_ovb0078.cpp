// base::reflection unit (tools/tu_map.py): three helpers of reflected records. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// base::reflection unit (tools/tu_map.py): fn_7100b7e8ac destroys a record: every non-null entry of the
// owned-pointer array at +0x18 (count +0x20, capacity +0x24) is destroyed (fn_7100b82ab0), deleted and
// cleared; the buffer goes back to the allocator at +0x38 (vf_0x48); then the file-path id at +0x8 is
// released. Names and layouts are placeholders.


struct UnkReflOwnedEntry_ovb0078;
void fn_7100b82ab0(UnkReflOwnedEntry_ovb0078* entry);

struct UnkReflEntryAllocator_ovb0078 {
    void** vtable;

    void vf_0x48(void* p) {
        reinterpret_cast<void (*)(UnkReflEntryAllocator_ovb0078*, void*)>(vtable[0x48 / 8])(this, p);
    }
};

struct UnkReflEntryRecord_ovb0078 {
    u8 pad_0x0[0x8];
    CFilePathStrId id;                      // 0x8
    u8 pad_0x10[0x8];
    UnkReflOwnedEntry_ovb0078** data;       // 0x18
    u32 count;                              // 0x20
    u32 capacity;                           // 0x24
    u8 pad_0x28[0x10];
    UnkReflEntryAllocator_ovb0078* alloc;   // 0x38
};

static inline void ReleasePath(CFilePathStrId* id) {
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
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

void fn_7100b7e8ac(UnkReflEntryRecord_ovb0078* self) {
    for (UnkReflOwnedEntry_ovb0078** it = self->data; it != self->data + self->count; ++it) {
        if (UnkReflOwnedEntry_ovb0078* entry = *it) {
            fn_7100b82ab0(entry);
            ::operator delete(entry);
            *it = nullptr;
        }
    }
    self->count = 0;
    if (UnkReflOwnedEntry_ovb0078** data = self->data) {
        self->count = 0;
        self->alloc->vf_0x48(data);
        self->capacity = 0;
    }
    ReleasePath(&self->id);
}

// base::reflection unit (tools/tu_map.py): fn_7100b7e994 finds the entry of a record's pointer array
// (+0x18, count +0x20) whose string id equals `key`, comparing through a copy of each entry's id;
// null when none does. Names and layouts are placeholders.


struct UnkReflIdEntry_ovb0078 {
    CStrId id;
};

struct UnkReflIdEntries_ovb0078 {
    u8 pad_0x0[0x18];
    UnkReflIdEntry_ovb0078** data;  // 0x18
    u32 count;                      // 0x20
};

static inline bool SameId(CStrId id, const CStrId& key) { return id.data == key.data; }

UnkReflIdEntry_ovb0078* fn_7100b7e994(UnkReflIdEntries_ovb0078* self, const CStrId* key) {
    for (UnkReflIdEntry_ovb0078** it = self->data; it != self->data + self->count; ++it) {
        if (SameId((*it)->id, *key))
            return *it;
    }
    return nullptr;
}

// base::reflection unit (tools/tu_map.py): fn_7100b7faf8 default-constructs a record laid out like
// fn_7100b7fbb0's UnkPathWords (a word, a file path, a word, a vector of 8-byte values): the path is
// built from g_unk_710162e2ed, the vector takes flags 0xc and reserves 40 entries. Names are placeholders.


struct UnkPathEntries_ovb0078 {
    u32 field_0x0 = 0;
    CFilePathStrId path{g_unk_710162e2ed, true};
    u64 field_0x10 = 0;
    base::UnkVector<u64> values{0xc};

    UnkPathEntries_ovb0078() { values.Reserve(0x28); }
};

void fn_7100b7faf8(UnkPathEntries_ovb0078* self) { new (self) UnkPathEntries_ovb0078(); }
