// Copy constructors of classes over a base copied out of line (fn_7100067ebc): after the base,
// each sets its vtable and copies its members in order (string ids and file paths with their
// references, runs of plain bytes as one block, arrays with their settings). Most have two
// identical copies. Some go through a middle class whose vtable is set first.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern char unk_7101aa0988[];  // the middle class's vtable
extern char unk_7101aa0958[];
extern char unk_7101aa5ad0[];

struct UnkIdBytesRecord {
    char* vtable;
    u8 unk_0x8[0x8];
    CStrId id;
    u8 field_0x18[3];
};

template <char* Vtable>
__attribute__((always_inline)) static inline void CopyRecord(UnkIdBytesRecord* self, const UnkIdBytesRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa0988 + 0x10;
    new (&self->id) CStrId(other->id);
    __builtin_memcpy(self->field_0x18, other->field_0x18, 3);
    self->vtable = Vtable + 0x10;
}

void fn_7100ba22f0(UnkIdBytesRecord* self, const UnkIdBytesRecord* other) { CopyRecord<unk_7101aa0958>(self, other); }
void fn_7100ba2370(UnkIdBytesRecord* self, const UnkIdBytesRecord* other) { CopyRecord<unk_7101aa0958>(self, other); }
void fn_7100cbe90c(UnkIdBytesRecord* self, const UnkIdBytesRecord* other) { CopyRecord<unk_7101aa5ad0>(self, other); }
void fn_7100cbe98c(UnkIdBytesRecord* self, const UnkIdBytesRecord* other) { CopyRecord<unk_7101aa5ad0>(self, other); }

// Classes directly over the base: N string ids from 0x10, then Bytes plain bytes.
template <u32 N, u32 Bytes>
struct UnkIdsRecord {
    char* vtable;
    u8 unk_0x8[0x8];
    CStrId ids[N];
    u8 data[Bytes];
};

template <char* Vtable, u32 N, u32 Bytes>
__attribute__((always_inline)) static inline void CopyIds(UnkIdsRecord<N, Bytes>* self, const UnkIdsRecord<N, Bytes>* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = Vtable + 0x10;
    for (u32 i = 0; i < N; ++i)
        new (&self->ids[i]) CStrId(other->ids[i]);
    if (Bytes)
        __builtin_memcpy(self->data, other->data, Bytes);
}

extern char unk_7101aa5500[];
extern char unk_7101aa5b60[];
extern char unk_7101aa6440[];
extern char unk_7101aa1708[];
extern char unk_7101aa1b88[];

#define COPY_IDS(addr, vtable, n, bytes)                                                 \
    void fn_##addr(UnkIdsRecord<n, bytes>* self, const UnkIdsRecord<n, bytes>* other) { \
        CopyIds<vtable, n, bytes>(self, other);                                          \
    }

COPY_IDS(7100cac920, unk_7101aa5500, 1, 0x2d)
COPY_IDS(7100cac9a0, unk_7101aa5500, 1, 0x2d)
COPY_IDS(7100cc074c, unk_7101aa5b60, 2, 0x1c)
COPY_IDS(7100cc07e0, unk_7101aa5b60, 2, 0x1c)
COPY_IDS(7100ce0aa4, unk_7101aa6440, 3, 0)
COPY_IDS(7100ce0b3c, unk_7101aa6440, 3, 0)
COPY_IDS(7100bca50c, unk_7101aa1708, 4, 4)
COPY_IDS(7100bca5c8, unk_7101aa1708, 4, 4)
COPY_IDS(7100bd8670, unk_7101aa1b88, 4, 0x15)
COPY_IDS(7100bd8734, unk_7101aa1b88, 4, 0x15)

// A file path at 0x10, string ids at 0x18 and 0x48 around 0x25 plain bytes.
extern char unk_7101aa5d48[];

struct UnkPathIdsRecord {
    char* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId path;
    CStrId id;
    u8 data[0x25];
    CStrId id2;
};

__attribute__((always_inline)) static inline void CopyPathIds(UnkPathIdsRecord* self, const UnkPathIdsRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa5d48 + 0x10;
    new (&self->path) CFilePathStrId(other->path);
    new (&self->id) CStrId(other->id);
    __builtin_memcpy(self->data, other->data, sizeof(self->data));
    new (&self->id2) CStrId(other->id2);
}

void fn_7100cc7c1c(UnkPathIdsRecord* self, const UnkPathIdsRecord* other) { CopyPathIds(self, other); }
void fn_7100cc7ce4(UnkPathIdsRecord* self, const UnkPathIdsRecord* other) { CopyPathIds(self, other); }

// An array of 8-byte values at 0x10 (base::UnkVector's layout: takes the source's settings,
// reserves its capacity and copies the elements), then 16 plain bytes.
extern char unk_7101aa1a68[];

struct UnkU64Array {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkU64Array(const UnkU64Array& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (capacity < n) {
            u64* p = static_cast<u64*>(allocator->vf_0x40((u64)n * sizeof(u64), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u64));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u64));
        count = other.count;
    }
};

struct UnkArrayRecord {
    char* vtable;
    u8 unk_0x8[0x8];
    UnkU64Array values;
    u8 data[0x10];
};

__attribute__((always_inline)) static inline void CopyArrayRecord(UnkArrayRecord* self, const UnkArrayRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa1a68 + 0x10;
    new (&self->values) UnkU64Array(other->values);
    __builtin_memcpy(self->data, other->data, sizeof(self->data));
}

void fn_7100bd4a48(UnkArrayRecord* self, const UnkArrayRecord* other) { CopyArrayRecord(self, other); }
void fn_7100bd4b38(UnkArrayRecord* self, const UnkArrayRecord* other) { CopyArrayRecord(self, other); }

// 16 plain bytes at 0xc, then eight string ids from 0x20.
extern char unk_7101aa16a8[];

struct UnkBytesIdsRecord {
    char* vtable;
    u8 unk_0x8[0x4];
    u8 data[0x10];
    CStrId ids[8];
};

__attribute__((always_inline)) static inline void CopyBytesIds(UnkBytesIdsRecord* self, const UnkBytesIdsRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa16a8 + 0x10;
    __builtin_memcpy(self->data, other->data, sizeof(self->data));
    for (u32 i = 0; i < 8; ++i)
        new (&self->ids[i]) CStrId(other->ids[i]);
}

void fn_7100bc7d14(UnkBytesIdsRecord* self, const UnkBytesIdsRecord* other) { CopyBytesIds(self, other); }
void fn_7100bc7e48(UnkBytesIdsRecord* self, const UnkBytesIdsRecord* other) { CopyBytesIds(self, other); }

// A middle class (vtable 0x7101aa1be8) of a file path, string ids and plain bytes, and the
// class's own file paths at 0x70..0x88.
extern char unk_7101aa1be8[];
extern char unk_7101aa1c90[];

struct UnkMixedRecord {
    char* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId path;
    u8 data_0x18[0xc];
    CStrId id_0x28;
    u8 data_0x30[1];
    CStrId id_0x38;
    CStrId id_0x40;
    u8 data_0x48[0x15];
    CStrId id_0x60;
    u8 data_0x68[4];
    CFilePathStrId paths[4];
};

void fn_7100bde12c(UnkMixedRecord* self, const UnkMixedRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa1be8 + 0x10;
    new (&self->path) CFilePathStrId(other->path);
    __builtin_memcpy(self->data_0x18, other->data_0x18, sizeof(self->data_0x18));
    new (&self->id_0x28) CStrId(other->id_0x28);
    __builtin_memcpy(self->data_0x30, other->data_0x30, sizeof(self->data_0x30));
    new (&self->id_0x38) CStrId(other->id_0x38);
    new (&self->id_0x40) CStrId(other->id_0x40);
    __builtin_memcpy(self->data_0x48, other->data_0x48, sizeof(self->data_0x48));
    new (&self->id_0x60) CStrId(other->id_0x60);
    __builtin_memcpy(self->data_0x68, other->data_0x68, sizeof(self->data_0x68));
    self->vtable = unk_7101aa1c90 + 0x10;
    for (u32 i = 0; i < 4; ++i)
        new (&self->paths[i]) CFilePathStrId(other->paths[i]);
}

// Plain bytes and string ids, then an array of string ids at 0x90 (base::UnkVector's layout,
// with an out-of-line Reserve, fn_710000542c): takes the source's settings, reserves its capacity
// and appends each id, growing like PushBack (including its hang).
extern char unk_7101aa5860[];

struct UnkStrIdArray {
    CStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
void fn_710000542c(UnkStrIdArray* self, u32 capacity);

struct UnkIdArrayRecord {
    char* vtable;
    u8 unk_0x8[0x4];
    u8 data_0xc[0x48];
    CStrId id_0x58;
    u8 data_0x60[0x18];
    CStrId id_0x78;
    u8 data_0x80[8];
    CStrId id_0x88;
    UnkStrIdArray ids;
};

void fn_7100cb8310(UnkIdArrayRecord* self, const UnkIdArrayRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa5860 + 0x10;
    __builtin_memcpy(self->data_0xc, other->data_0xc, sizeof(self->data_0xc));
    new (&self->id_0x58) CStrId(other->id_0x58);
    __builtin_memcpy(self->data_0x60, other->data_0x60, sizeof(self->data_0x60));
    new (&self->id_0x78) CStrId(other->id_0x78);
    __builtin_memcpy(self->data_0x80, other->data_0x80, sizeof(self->data_0x80));
    new (&self->id_0x88) CStrId(other->id_0x88);
    UnkStrIdArray* ids = &self->ids;
    ids->data = nullptr;
    ids->count = 0;
    ids->capacity = 0;
    ids->step = other->ids.step;
    ids->field_0x14 = other->ids.field_0x14;
    ids->flags = other->ids.flags;
    ids->allocator = other->ids.allocator;
    fn_710000542c(ids, other->ids.capacity);
    for (const CStrId *it = other->ids.data, *end = it + other->ids.count; it != end; ++it) {
        u32 n = ids->count;
        u32 new_capacity = ids->capacity;
        if (new_capacity <= n && ids->step != 0)
            new_capacity = (n / ids->step + 1) * ids->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_710000542c(ids, new_capacity);
        // A bitwise copy of the id (the original reloads the count after it), then the reference.
        CStrId* slot = &ids->data[ids->count];
        base::global::UnkStringData* data = it->data;
        __builtin_memcpy(slot, &data, sizeof(data));
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
        ++ids->count;
    }
}

// Eight file paths, then string ids around plain bytes.
extern char unk_7101aa63e0[];

struct UnkPathsRecord {
    char* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId paths[8];
    CStrId id_0x50;
    u8 data_0x58[0x2a];
    CStrId id_0x88;
    CStrId id_0x90;
    u8 data_0x98[4];
};

void fn_7100cded48(UnkPathsRecord* self, const UnkPathsRecord* other) {
    fn_7100067ebc(reinterpret_cast<char*>(self), reinterpret_cast<const char*>(other));
    self->vtable = unk_7101aa63e0 + 0x10;
    for (u32 i = 0; i < 8; ++i)
        new (&self->paths[i]) CFilePathStrId(other->paths[i]);
    new (&self->id_0x50) CStrId(other->id_0x50);
    __builtin_memcpy(self->data_0x58, other->data_0x58, sizeof(self->data_0x58));
    new (&self->id_0x88) CStrId(other->id_0x88);
    new (&self->id_0x90) CStrId(other->id_0x90);
    __builtin_memcpy(self->data_0x98, other->data_0x98, sizeof(self->data_0x98));
}
