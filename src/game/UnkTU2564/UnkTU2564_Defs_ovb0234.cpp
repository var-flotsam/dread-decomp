// Unit 2564 (CMinimapDef by reflection hint; tools/tu_map.py gives it no reliable name): the minimap
// definitions' tables of string-id keyed records. The 0xc8-byte record (three string ids, a word, a
// string with 0x40 bytes of storage and plain values; fn_7100e6c960 destroys it) and the 0x18-byte one
// (an id and four words) in arrays with a key array alongside: growth and copy of the 0xc8 array, the
// swap-erase of both, the record's copy assignment, the moves of the iterator values that hold one, the
// object that owns a table of each (constructor and destructor), the registry of icon kinds, the kind
// lookup and reading a reflected resource from its file. Names and layouts are placeholders (siblings:
// src/game/CMinimapDef_ShrinkToFit_b49.cpp, src/game/sound/CSoundManager/CSoundManager_tables_ovb0038.cpp).
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkCountedResource.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/core/CDefinition.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2564.h"
#include "game/UnkTU32.h"
#include "types.h"
#include "base/UnkStrPair.h"
#include "base/UnkFileReader.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflObject;

// ---- Reading a reflected resource from the file its path names ----

// The id and path pair built from a resource's path, destroyed out of line.
struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    ~UnkPathPairTemp_b45a1() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71727c8d90 __attribute__((visibility("hidden")));

// A reflected resource and its path.
struct UnkMinimapResource_b0234 : UnkReflObject {
    u8 path[0x50];  // 0x8
};

struct UnkLoadRequest_b0234 {
    u8 unk_0x0[0x10];
    u32 mode;
};

// Parses the file into the target, starting from its last entry's node; nonzero on success.
static inline s32 Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71727c8d90, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(
        &ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Reads the resource's reflected object from the file its path names; whether it parsed.
bool fn_7100e6bd40(UnkMinimapResource_b0234* self, const UnkLoadRequest_b0234* request) {
    UnkFileReader reader;
    {
        UnkPathPairTemp_b45a1 s = fn_710001ab24(self->path);
        fn_710007be8c(&reader.file, 0x16, nullptr);
        reader.field_0x38 = 3;
        reader.field_0x3c = 2;
        bool opened;
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(&reader, reinterpret_cast<UnkStrPair_0x10*>(&s), &mode);
        }
        if (opened)
            fn_710000ccd8(&reader);
    }
    UnkReadTarget target;
    {
        u32 mode = request->mode;
        const base::reflection::CClass* type = self->vf_0x0();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, self, true);
        fn_71000f0568(&target, &value, mode);
        fn_71000a1c60(&value);
    }
    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        ok = Parse(&reader, &target) != 0;
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
    return ok;
}

// ---- The registry of icon kinds ----

// Every kind made, with its name (two parallel arrays), destroyed at exit (fn_7100e7241c).
struct UnkKindRegistry_b0234 {
    CStrId** kinds;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkKindRegistry_b0234()
        : kinds(nullptr), ids(nullptr), count(0), capacity(0), step(10), flags(0xe), allocator(fn_7100083990()) {}
    ~UnkKindRegistry_b0234() asm("fn_7100e7241c");
};
void fn_7100e72518(UnkKindRegistry_b0234* self, u32 capacity);

inline UnkKindRegistry_b0234& KindRegistry_b0234() {
    static UnkKindRegistry_b0234 s_registry;
    return s_registry;
}

// Copies a kind's name into it and adds it to the registry.
void fn_7100e68cac(CStrId* kind, const CStrId& name) {
    new (kind) CStrId(name);
    UnkKindRegistry_b0234& registry = KindRegistry_b0234();
    if (registry.count == registry.capacity)
        fn_7100e72518(&registry, registry.count + registry.step);
    CStrId* slot = &registry.ids[registry.count];
    __builtin_memcpy(slot, kind, sizeof(CStrId));
    if (&g_unk_7101d47108->empty != slot->data)
        ++slot->data->ref_count;
    __builtin_memcpy(&registry.kinds[registry.count], &kind, sizeof(kind));
    ++registry.count;
}

// ---- The kind lookup ----

s32 fn_7100e68e38(const CStrId* id) {
    if (id->data == g_unk_71727c82d8.data)
        return 6;
    if (id->data == g_unk_71727c82e0.data)
        return 5;
    if (id->data == g_unk_71727c82e8.data)
        return 7;
    if (id->data == g_unk_71727c82f0.data)
        return 2;
    if (id->data == g_unk_71727c82f8.data)
        return 1;
    if (id->data == g_unk_71727c8300.data)
        return 0;
    if (id->data == g_unk_71727c8308.data)
        return 3;
    if (id->data == g_unk_71727c8310.data)
        return 4;
    return 0x7fffffff;
}

// ---- A string id built empty, and a constructor from it ----

// The pointer is cleared before the pool is made sure of.
struct UnkEmptyId_b0234 {
    UnkStringData* data;

    UnkEmptyId_b0234() : data(nullptr) {
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

// An id and two words, value-initialised (zeroed, then the empty id and the words set).
struct UnkIdTail_b0234 {
    UnkEmptyId_b0234 id;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
};

void fn_7100e6e4a0(void* self) { new (self) UnkIdTail_b0234(); }

// ---- The records ----

// CStrId's copy constructor, with the pointer copied untyped (the count is reloaded after it).
__attribute__((always_inline)) static inline void CopyId(CStrId* dst, const CStrId* src) {
    UnkStringData* data = src->data;
    __builtin_memcpy(&dst->data, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

// A string id taken over, the moved-from one left empty.
struct UnkMovedStrId_b0234 {
    UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId_b0234(UnkMovedStrId_b0234&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

extern void* g_unk_7101a1b678[];
using UnkString40_b0234 = UnkBufferString<0x40, g_unk_7101a1b678>;

// Copied as a block.
struct UnkWord_b0234 {
    u32 value;

    __attribute__((always_inline)) UnkWord_b0234(const UnkWord_b0234& other) { __builtin_memcpy(this, &other, 4); }
};

// Copied as a 0x14-byte block.
struct UnkWords5_b0234 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;

    __attribute__((always_inline)) UnkWords5_b0234(const UnkWords5_b0234& other) {
        __builtin_memcpy(this, &other, 0x14);
    }
    UnkWords5_b0234& operator=(const UnkWords5_b0234& other) {
        __builtin_memcpy(this, &other, 0x14);
        return *this;
    }
};

// The same block copied through loads (the original loads in a path of its own and merges only the
// stores).
struct UnkWords5Typed_b0234 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;

    __attribute__((always_inline)) UnkWords5Typed_b0234(const UnkWords5Typed_b0234& other) {
        u64 a = other.field_0x0;
        u64 b = other.field_0x8;
        u32 c = other.field_0x10;
        field_0x10 = c;
        field_0x0 = a;
        field_0x8 = b;
    }
};

// The 0xc8-byte record with its ids copied on a copy.
struct UnkIdsString40Copy_b0234 {
    CStrId field_0x0;
    u64 field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    UnkString40_b0234 field_0x20;
    UnkWord_b0234 field_0xa8;
    UnkWord_b0234 field_0xac;
    UnkWords5_b0234 field_0xb0;
};
static_assert(sizeof(UnkIdsString40Copy_b0234) == 0xc8, "layout");

// The same with its ids taken over on a move.
struct UnkIdsString40_b0234 {
    UnkMovedStrId_b0234 field_0x0;
    u64 field_0x8;
    UnkMovedStrId_b0234 field_0x10;
    UnkMovedStrId_b0234 field_0x18;
    UnkString40_b0234 field_0x20;
    UnkWord_b0234 field_0xa8;
    UnkWord_b0234 field_0xac;
    UnkWords5_b0234 field_0xb0;
};
static_assert(sizeof(UnkIdsString40_b0234) == 0xc8, "layout");

// As above, the block of words at +0xb0 copied through loads.
struct UnkIdsString40Typed_b0234 {
    UnkMovedStrId_b0234 field_0x0;
    u64 field_0x8;
    UnkMovedStrId_b0234 field_0x10;
    UnkMovedStrId_b0234 field_0x18;
    UnkString40_b0234 field_0x20;
    UnkWord_b0234 field_0xa8;
    UnkWord_b0234 field_0xac;
    UnkWords5Typed_b0234 field_0xb0;
};

// The record's copy assignment.
UnkIdsString40Copy_b0234* fn_7100e6e1b0(UnkIdsString40Copy_b0234* self, const UnkIdsString40Copy_b0234* other) {
    self->field_0x0 = other->field_0x0;
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    fn_710007eaa0(self->field_0x20.Str(), other->field_0x20.Str());
    self->field_0xa8.value = other->field_0xa8.value;
    self->field_0xac.value = other->field_0xac.value;
    self->field_0xb0 = other->field_0xb0;
    return self;
}

// ---- The keyed arrays ----

// A 0x18-byte record: a string id taken over on a move (released in two steps when destroyed) and
// four words.
struct UnkIdWords4_b0234 {
    UnkMovedStrId_b0234 id;
    UnkWord_b0234 field_0x8;
    UnkWord_b0234 field_0xc;
    UnkWord_b0234 field_0x10;
    UnkWord_b0234 field_0x14;

    UnkIdWords4_b0234(UnkIdWords4_b0234&& other) = default;
    ~UnkIdWords4_b0234() { reinterpret_cast<CStrId*>(&id)->Reset(); }
};
static_assert(sizeof(UnkIdWords4_b0234) == 0x18, "layout");

// Records and string-id keys in two arrays sharing one count.
template <class T>
struct UnkKeyedArray_b0234 {
    T* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// A key as the growth moves it (as src/game/CMinimapDef_ShrinkToFit_b49.cpp's UnkKeyId_b49): the
// moved-from id is cleared through a union, then released in two steps.
union UnkKeyIdSlot_b0234 {
    UnkStringData* p;
};

struct UnkKeyId_b0234 {
    UnkStringData* data;

    UnkKeyId_b0234(UnkKeyId_b0234&& other) : data(other.data) {
        reinterpret_cast<UnkKeyIdSlot_b0234*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    ~UnkKeyId_b0234() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline UnkKeyId_b0234* Keys(UnkKeyedArray_b0234<UnkIdsString40_b0234>* self) {
    return reinterpret_cast<UnkKeyId_b0234*>(self->keys);
}
static inline UnkKeyId_b0234* Keys(UnkKeyedArray_b0234<UnkIdWords4_b0234>* self) {
    return reinterpret_cast<UnkKeyId_b0234*>(self->keys);
}

// Grows the array of 0xc8-byte records: the records are moved over and destroyed, the keys taken
// over, and the old arrays freed.
void fn_7100e6f940(void* array, u32 capacity) {
    auto* self = static_cast<UnkKeyedArray_b0234<UnkIdsString40_b0234>*>(array);
    if (self->capacity >= capacity)
        return;
    auto* values = static_cast<UnkIdsString40_b0234*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkIdsString40_b0234), 8, self->flags));
    auto* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
    if (UnkIdsString40_b0234* src = self->values) {
        UnkIdsString40_b0234* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkIdsString40_b0234(static_cast<UnkIdsString40_b0234&&>(*src));
            fn_7100e6c960(reinterpret_cast<char*>(src));
        }
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->keys) {
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
            new (&keys[j]) UnkKeyId_b0234(static_cast<UnkKeyId_b0234&&>(*reinterpret_cast<UnkKeyId_b0234*>(old)));
            reinterpret_cast<UnkKeyId_b0234*>(old)->~UnkKeyId_b0234();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}

// The copy constructor of the array of 0xc8-byte records: the growth parameters copied, room for the
// other's count reserved, then the records and the ids copied.
void fn_7100e6f780(UnkKeyedArray_b0234<UnkIdsString40Copy_b0234>* self,
                   const UnkKeyedArray_b0234<UnkIdsString40Copy_b0234>* other) {
    self->values = nullptr;
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100e6f940(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        new (&self->values[i]) UnkIdsString40Copy_b0234(other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        CopyId(&self->keys[i], &other->keys[i]);
}

// Removes the record at `pos` and its key: the last record and key are moved into its place.
// Returns the record now at that place.
UnkIdsString40_b0234* fn_7100e70438(UnkKeyedArray_b0234<UnkIdsString40_b0234>* self, UnkIdsString40_b0234* pos) {
    u32 i = pos - self->values;
    fn_7100e6c960(reinterpret_cast<char*>(&self->values[i]));
    self->keys[i].Reset();
    if (i != self->count - 1) {
        new (&self->values[i]) UnkIdsString40_b0234(static_cast<UnkIdsString40_b0234&&>(self->values[self->count - 1]));
        fn_7100e6c960(reinterpret_cast<char*>(&self->values[self->count - 1]));
        new (&Keys(self)[i]) UnkKeyId_b0234(static_cast<UnkKeyId_b0234&&>(Keys(self)[self->count - 1]));
        Keys(self)[self->count - 1].~UnkKeyId_b0234();
    }
    --self->count;
    return &self->values[i];
}

// The same for the array of 0x18-byte records, destroyed and moved inline.
UnkIdWords4_b0234* fn_7100e72018(UnkKeyedArray_b0234<UnkIdWords4_b0234>* self, UnkIdWords4_b0234* pos) {
    u32 i = pos - self->values;
    self->values[i].~UnkIdWords4_b0234();
    Keys(self)[i].~UnkKeyId_b0234();
    if (i != self->count - 1) {
        new (&self->values[i]) UnkIdWords4_b0234(static_cast<UnkIdWords4_b0234&&>(self->values[self->count - 1]));
        self->values[self->count - 1].~UnkIdWords4_b0234();
        new (&Keys(self)[i]) UnkKeyId_b0234(static_cast<UnkKeyId_b0234&&>(Keys(self)[self->count - 1]));
        Keys(self)[self->count - 1].~UnkKeyId_b0234();
    }
    --self->count;
    return &self->values[i];
}

// ---- The iterator values that hold a (key, record) pair ----

// A pair held in place (state 1) or by reference (state 2).
template <class X, class Y>
struct UnkPairRef_b0234 {
    union {
        struct {
            X x;
            Y y;
        } values;
        struct {
            X* x;
            Y* y;
        } refs;
    };
    s32 state;
};

using UnkMovedPair_b0234 = UnkPairRef_b0234<UnkMovedStrId_b0234, UnkIdsString40_b0234>;

// Both ways end in the pair held in place. The in-place record's words are copied through loads, so
// the two paths keep their own loads (the stores merge).
__attribute__((always_inline)) static inline void MoveConstruct(UnkMovedPair_b0234* self, UnkMovedPair_b0234* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        self->state = 1;
        new (&self->values.x) UnkMovedStrId_b0234(static_cast<UnkMovedStrId_b0234&&>(other->values.x));
        new (&self->values.y) UnkIdsString40Typed_b0234(
            static_cast<UnkIdsString40Typed_b0234&&>(reinterpret_cast<UnkIdsString40Typed_b0234&>(other->values.y)));
        break;
    case 2: {
        self->state = 1;
        UnkMovedStrId_b0234* x = other->refs.x;
        UnkIdsString40_b0234* y = other->refs.y;
        new (&self->values.x) UnkMovedStrId_b0234(static_cast<UnkMovedStrId_b0234&&>(*x));
        new (&self->values.y) UnkIdsString40_b0234(static_cast<UnkIdsString40_b0234&&>(*y));
        break;
    }
    }
}

void fn_7100e6eb28(UnkMovedPair_b0234* self, UnkMovedPair_b0234* other) { MoveConstruct(self, other); }

using UnkCopyPair_b0234 = UnkPairRef_b0234<UnkMovedStrId_b0234, UnkIdsString40Copy_b0234>;

// The key is taken over, the record copied.
__attribute__((always_inline)) static inline void CopyConstruct(UnkCopyPair_b0234* self, UnkCopyPair_b0234* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        self->state = 1;
        new (&self->values.x) UnkMovedStrId_b0234(static_cast<UnkMovedStrId_b0234&&>(other->values.x));
        new (&self->values.y) UnkIdsString40Copy_b0234(other->values.y);
        break;
    case 2: {
        self->state = 1;
        UnkMovedStrId_b0234* x = other->refs.x;
        UnkIdsString40Copy_b0234* y = other->refs.y;
        new (&self->values.x) UnkMovedStrId_b0234(static_cast<UnkMovedStrId_b0234&&>(*x));
        new (&self->values.y) UnkIdsString40Copy_b0234(*y);
        break;
    }
    }
}

void fn_7100e6ef18(void* self, const void* other) {
    CopyConstruct(static_cast<UnkCopyPair_b0234*>(self),
                  const_cast<UnkCopyPair_b0234*>(static_cast<const UnkCopyPair_b0234*>(other)));
}

// ---- The object that owns a table of each ----

extern void* g_unk_7101aa9d58[];

void fn_7100025398(void* self);  // the base's constructor

// A string id, a file-path id and the resource the path resolves to, starting empty.
struct UnkIdPathResource_b0234 {
    CStrId id;
    CFilePathStrId path;
    UnkCountedResource* resource;

    UnkIdPathResource_b0234() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), resource(nullptr) {}
};
static_assert(sizeof(UnkIdPathResource_b0234) == 0x18, "layout");

// Two parallel arrays (records, string-id keys) sharing a count, built empty.
struct UnkKeyedArrayRaw_b0234 {
    void* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkKeyedArrayRaw_b0234()
        : values(nullptr), keys(nullptr), count(0), capacity(0), step(10), flags(0xe), allocator(fn_7100083990()) {}
};

struct UnkMinimapDef_b0234 {
    void** vtable;
    u8 unk_0x8[0x58];
    UnkIdPathResource_b0234 field_0x60;
    UnkIdPathResource_b0234 field_0x78;
    UnkIdPathResource_b0234 field_0x90;
    UnkKeyedArrayRaw_b0234 field_0xa8;  // the 0xc8-byte records
    UnkKeyedArrayRaw_b0234 field_0xd0;  // the 0x18-byte records

    UnkMinimapDef_b0234() : vtable(g_unk_7101aa9d58 + 2) {}
};
static_assert(sizeof(UnkMinimapDef_b0234) == 0xf8, "layout");

__attribute__((flatten)) void fn_7100e6ba6c(UnkMinimapDef_b0234* self) {
    fn_7100025398(self);
    new (self) UnkMinimapDef_b0234();
}

// A string id released in two steps (the original reloads the id after the decrement).
struct UnkReleasedStrId_b0234 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId_b0234() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdPair;
void fn_7100e71cc4(UnkKeyedArrayRaw_b0234* array);  // destroys the 0x18-byte records and their keys

__attribute__((flatten)) void fn_7100e6bb90(UnkMinimapDef_b0234* self) {
    self->vtable = g_unk_7101aa9d58 + 2;
    UnkKeyedArrayRaw_b0234* b = &self->field_0xd0;
    fn_7100e71cc4(b);
    if (b->keys)
        b->allocator->vf_0x48(b->keys);
    if (b->values)
        b->allocator->vf_0x48(b->values);
    UnkKeyedArrayRaw_b0234* a = &self->field_0xa8;
    if (a->keys && a->count) {
        for (u32 i = 0; i < a->count; ++i)
            reinterpret_cast<UnkReleasedStrId_b0234*>(a->keys)[i].~UnkReleasedStrId_b0234();
    }
    if (a->values && a->count) {
        for (u32 i = 0; i < a->count; ++i)
            fn_7100e6c960(static_cast<char*>(a->values) + (u64)i * 0xc8);
    }
    a->count = 0;
    if (a->keys)
        a->allocator->vf_0x48(a->keys);
    if (a->values)
        a->allocator->vf_0x48(a->values);
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x90));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x78));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->field_0x60));
    fn_710002545c(self);
}
