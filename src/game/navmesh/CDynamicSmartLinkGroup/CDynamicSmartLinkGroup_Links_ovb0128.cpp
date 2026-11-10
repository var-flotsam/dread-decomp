// navmesh::CDynamicSmartLinkGroup's unit (tools/tu_map.py, unit 351): the group's array of links (a file path
// and a counted reference to a resource, 0x10 bytes): its copy, growing access, erase, and the group's
// destructor. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using namespace base::global;

// A file-path id (on CFilePathStrId's pool) that moves, and whose release reloads the id and its
// count after the decrement.
struct UnkLinkPath_ovb0128 : UnkFilePathStrIdBase {
    UnkLinkPath_ovb0128() {
        data = nullptr;
        if (!g_unk_7101d47200)
            UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    UnkLinkPath_ovb0128(const UnkLinkPath_ovb0128& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkLinkPath_ovb0128(UnkLinkPath_ovb0128&& other) : UnkFilePathStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkLinkPath_ovb0128() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A counted reference to a resource (base/UnkCountedResource.h); moves copy it.
struct UnkSmartLinkRef_ovb0128 {
    UnkCountedResource* ptr;

    UnkSmartLinkRef_ovb0128() : ptr(nullptr) {}
    UnkSmartLinkRef_ovb0128(const UnkSmartLinkRef_ovb0128& other) : ptr(nullptr) {
        if (UnkCountedResource* p = other.ptr)
            Assign(p);
    }
    ~UnkSmartLinkRef_ovb0128() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }

    // Takes a reference to p (not null), dropping the held one.
    void Assign(UnkCountedResource* p) {
        ++p->ref_count;
        if (UnkCountedResource* old = ptr) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        ptr = p;
    }
};

struct UnkSmartLinkEntry_ovb0128 {
    UnkLinkPath_ovb0128 path;
    UnkSmartLinkRef_ovb0128 object;
};

// Grows the array to at least `capacity` entries (out of line; src/base/UnkUnit259/UnkNamedRefVector_Reserve_b58.cpp).
void fn_71002682e8(base::UnkVector<u8>* vector, u32 capacity);

static inline void Grow(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self, u32 capacity) {
    fn_71002682e8(reinterpret_cast<base::UnkVector<u8>*>(self), capacity);
}

// Reserves the other's capacity and appends a copy of each of its entries (the growth is out of line).
static inline void AppendAll(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self,
                             const base::UnkVector<UnkSmartLinkEntry_ovb0128>* other) {
    Grow(self, other->capacity);
    for (const UnkSmartLinkEntry_ovb0128 *it = other->data, *end = it + other->count; it != end; ++it) {
        u32 n = self->count;
        u32 cap = self->capacity;
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)cap)
            for (;;) {
            }
        Grow(self, cap);
        new (&self->data[self->count]) UnkSmartLinkEntry_ovb0128(*it);
        ++self->count;
    }
}

// Copy-constructs the array: the other's settings, its capacity reserved, then each entry appended.
void fn_71002681a8(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self,
                   const base::UnkVector<UnkSmartLinkEntry_ovb0128>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    AppendAll(self, other);
}

// Copy-assignment: the entries destroyed, then the other's appended.
base::UnkVector<UnkSmartLinkEntry_ovb0128>* fn_7100268474(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self,
                                                          const base::UnkVector<UnkSmartLinkEntry_ovb0128>* other) {
    if (self != other) {
        self->Clear();
        AppendAll(self, other);
    }
    return self;
}

// Destroys the entry at `index` and moves the later ones down.
__attribute__((flatten)) void fn_7100268ab8(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkSmartLinkEntry_ovb0128();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkSmartLinkEntry_ovb0128* cur = &self->data[i];
            new (cur) UnkSmartLinkEntry_ovb0128(static_cast<UnkSmartLinkEntry_ovb0128&&>(cur[1]));
            (&self->data[i])[1].~UnkSmartLinkEntry_ovb0128();
        }
        self->count = self->count - 1;
    }
}

// Puts `value` in at `index` (moved for fn_7100268898, copied for fn_7100268654), shifting the
// entries after it up; appending hangs when the array cannot grow, as base::UnkVector::Insert.
template <class V>
static inline void InsertAt(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self, V&& value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        if (cap <= n && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        if ((s32)cap <= (s32)index)
            for (;;) {
            }
        Grow(self, cap);
        new (&self->data[self->count]) UnkSmartLinkEntry_ovb0128(static_cast<V&&>(value));
    } else {
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        Grow(self, cap);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkSmartLinkEntry_ovb0128* q = &self->data[i];
            new (q + 1) UnkSmartLinkEntry_ovb0128(static_cast<UnkSmartLinkEntry_ovb0128&&>(*q));
            self->data[i].~UnkSmartLinkEntry_ovb0128();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkSmartLinkEntry_ovb0128(static_cast<V&&>(value));
    }
    ++self->count;
}

void fn_7100268898(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self, UnkSmartLinkEntry_ovb0128* value, u32 index) {
    InsertAt(self, static_cast<UnkSmartLinkEntry_ovb0128&&>(*value), index);
}

void fn_7100268654(base::UnkVector<UnkSmartLinkEntry_ovb0128>* self, const UnkSmartLinkEntry_ovb0128* value,
                   u32 index) {
    InsertAt(self, *value, index);
}

// The array as CDynamicSmartLinkGroup_MapSet_b49.cpp names it (its declaration of fn_7100268c64).
struct UnkSmartLinkMap_b49a1;

// The entry at `index`; when growing is allowed (field_0x14), an index past the end grows the array
// to the step's next multiple and then to index + 1 entries, value-initialising the new ones.
UnkSmartLinkEntry_ovb0128* fn_7100268c64(UnkSmartLinkMap_b49a1* map, u32 index) {
    base::UnkVector<UnkSmartLinkEntry_ovb0128>* self = reinterpret_cast<base::UnkVector<UnkSmartLinkEntry_ovb0128>*>(map);
    if (self->field_0x14 && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        Grow(self, cap);
        u32 n = index + 1;
        Grow(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkSmartLinkEntry_ovb0128();
        self->count = n;
    }
    return &self->data[index];
}

void fn_7100016f88(void* self);
extern void* g_unk_7101a20cc0[];

// The group: a file path at +0x58 and the links at +0x60, over the 0x58-byte base (fn_7100016f88).
struct UnkSmartLinkGroup_ovb0128 {
    void** vtable;
    u8 base_0x8[0x50];
    UnkLinkPath_ovb0128 path;                           // 0x58
    base::UnkVector<UnkSmartLinkEntry_ovb0128> links;   // 0x60

    __attribute__((always_inline)) ~UnkSmartLinkGroup_ovb0128() {
        vtable = &g_unk_7101a20cc0[2];
        links.Free();
    }
};
static_assert(__builtin_offsetof(UnkSmartLinkGroup_ovb0128, links) == 0x60, "layout");

// The destructor.
void fn_7100266290(UnkSmartLinkGroup_ovb0128* self) {
    self->~UnkSmartLinkGroup_ovb0128();
    fn_7100016f88(self);
}
