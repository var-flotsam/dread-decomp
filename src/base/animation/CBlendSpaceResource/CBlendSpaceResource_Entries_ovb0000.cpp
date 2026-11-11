// base::animation::CBlendSpaceResource's unit (tools/tu_map.py unit 14): the resource's array of 0x18-byte entries
// (a value, a file-path id and a counted resource reference, base::UnkVector's layout) with its out-of-line reserve
// (fn_710000e610), push-back (fn_710000e7a4), copy-assignment (fn_710000e890), inserts by copy (fn_710000e9c0) and by
// move (fn_710000ebe0), erase by index (fn_710000ee20) and by index iterator (fn_710000e08c), growing element access
// (fn_710000efe0), and the resource's destructor (fn_710000b66c). Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file path whose move leaves the source empty.
struct UnkBlendPath_ovb0000 : base::global::UnkFilePathStrIdBase {
    // The empty path (as CFilePathStrId's default constructor).
    UnkBlendPath_ovb0000() {
        data = nullptr;
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    UnkBlendPath_ovb0000(const UnkBlendPath_ovb0000& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkBlendPath_ovb0000(UnkBlendPath_ovb0000&& other) : UnkFilePathStrIdBase(other) {
        other.data = &g_unk_7101d47200->empty;
    }
    ~UnkBlendPath_ovb0000() {
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

// A counted reference to a resource, copied by assignment.
struct UnkBlendResRef_ovb0000 {
    UnkCountedResource* ptr;

    UnkBlendResRef_ovb0000() : ptr(nullptr) {}
    UnkBlendResRef_ovb0000(const UnkBlendResRef_ovb0000& other) : ptr(nullptr) { *this = other; }
    UnkBlendResRef_ovb0000& operator=(const UnkBlendResRef_ovb0000& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkBlendResRef_ovb0000() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

// A blend space entry (0x18 bytes): a value, a path and the resource loaded from it.
// The first word is copied bytewise (char alias info), so a move reloads the file-path pool.
struct UnkBlendEntry_ovb0000 {
    u8 field_0x0[8];
    UnkBlendPath_ovb0000 path;
    UnkBlendResRef_ovb0000 resource;
};
static_assert(sizeof(UnkBlendEntry_ovb0000) == 0x18, "entry layout");

using UnkBlendEntries_ovb0000 = base::UnkVector<UnkBlendEntry_ovb0000>;

// As src/base/animation/CBlendSpaceResource/fn_710000e2ec.cpp declares them (UnkElem18 is the entry).
struct UnkElem18;
struct UnkElem18Vector;

void fn_710000e610(UnkElem18Vector* self, u32 n) {
    UnkBlendEntries_ovb0000* v = reinterpret_cast<UnkBlendEntries_ovb0000*>(self);
    if (v->capacity >= n)
        return;
    UnkBlendEntry_ovb0000* p = static_cast<UnkBlendEntry_ovb0000*>(
        v->allocator->vf_0x40((u64)n * sizeof(UnkBlendEntry_ovb0000), 8, v->flags));
    if (v->data) {
        UnkBlendEntry_ovb0000* src = v->data;
        UnkBlendEntry_ovb0000* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkBlendEntry_ovb0000(static_cast<UnkBlendEntry_ovb0000&&>(*src));
            src->~UnkBlendEntry_ovb0000();
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = p;
    v->capacity = n;
}

void fn_710000e7a4(UnkElem18Vector* self, const UnkElem18* element) {
    UnkBlendEntries_ovb0000* v = reinterpret_cast<UnkBlendEntries_ovb0000*>(self);
    const UnkBlendEntry_ovb0000* value = reinterpret_cast<const UnkBlendEntry_ovb0000*>(element);
    u32 n = v->count;
    u32 new_capacity = v->capacity;
    if (new_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_710000e610(self, new_capacity);
    new (&v->data[v->count]) UnkBlendEntry_ovb0000(*value);
    ++v->count;
}

void fn_710000ee20(UnkElem18Vector* self, u32 index) {
    UnkBlendEntries_ovb0000* v = reinterpret_cast<UnkBlendEntries_ovb0000*>(self);
    if (index < v->count) {
        v->data[index].~UnkBlendEntry_ovb0000();
        for (u64 i = index; i < v->count - 1; ++i) {
            UnkBlendEntry_ovb0000* e = &v->data[i];
            new (e) UnkBlendEntry_ovb0000(static_cast<UnkBlendEntry_ovb0000&&>(e[1]));
            (&v->data[i])[1].~UnkBlendEntry_ovb0000();
        }
        v->count = v->count - 1;
    }
}

UnkElem18Vector* fn_710000e890(UnkElem18Vector* self, const UnkElem18Vector* other) {
    if (self != other) {
        UnkBlendEntries_ovb0000* v = reinterpret_cast<UnkBlendEntries_ovb0000*>(self);
        const UnkBlendEntries_ovb0000* o = reinterpret_cast<const UnkBlendEntries_ovb0000*>(other);
        for (u32 i = 0; i < v->count; ++i)
            v->data[i].~UnkBlendEntry_ovb0000();
        v->count = 0;
        fn_710000e610(self, o->capacity);
        for (const UnkBlendEntry_ovb0000 *it = o->data, *end = it + o->count; it != end; ++it)
            fn_710000e7a4(self, reinterpret_cast<const UnkElem18*>(it));
    }
    return self;
}

void fn_710000e9c0(UnkElem18Vector* self, const UnkBlendEntry_ovb0000* value, u32 index) {
    UnkBlendEntries_ovb0000* v = reinterpret_cast<UnkBlendEntries_ovb0000*>(self);
    u32 n = v->count;
    if (n == index) {
        fn_710000e7a4(self, reinterpret_cast<const UnkElem18*>(value));
        return;
    }
    u32 capacity = v->capacity;
    if (capacity <= n && v->step != 0)
        capacity = (n / v->step + 1) * v->step;
    fn_710000e610(self, capacity);
    for (u32 i = v->count - 1; i >= index; --i) {
        UnkBlendEntry_ovb0000* e = &v->data[i];
        new (e + 1) UnkBlendEntry_ovb0000(static_cast<UnkBlendEntry_ovb0000&&>(*e));
        v->data[i].~UnkBlendEntry_ovb0000();
        if (i == 0)
            break;
    }
    new (&v->data[index]) UnkBlendEntry_ovb0000(*value);
    ++v->count;
}

void fn_710000ebe0(UnkElem18Vector* self, UnkBlendEntry_ovb0000* value, u32 index) {
    UnkBlendEntries_ovb0000* v = reinterpret_cast<UnkBlendEntries_ovb0000*>(self);
    u32 capacity = v->capacity;
    u32 n = v->count;
    if (n == index) {
        if (capacity <= n && v->step != 0)
            capacity = (index / v->step + 1) * v->step;
        if ((s32)capacity <= (s32)index)
            for (;;) {
            }
        fn_710000e610(self, capacity);
        new (&v->data[v->count]) UnkBlendEntry_ovb0000(static_cast<UnkBlendEntry_ovb0000&&>(*value));
        ++v->count;
        return;
    } else {
        if (capacity <= n && v->step != 0)
            capacity = (n / v->step + 1) * v->step;
        fn_710000e610(self, capacity);
        for (u32 i = v->count - 1; i >= index; --i) {
            UnkBlendEntry_ovb0000* e = &v->data[i];
            new (e + 1) UnkBlendEntry_ovb0000(static_cast<UnkBlendEntry_ovb0000&&>(*e));
            v->data[i].~UnkBlendEntry_ovb0000();
            if (i == 0)
                break;
        }
    }
    new (&v->data[index]) UnkBlendEntry_ovb0000(static_cast<UnkBlendEntry_ovb0000&&>(*value));
    ++v->count;
}

UnkBlendEntry_ovb0000* fn_710000efe0(UnkElem18Vector* self, u32 index) {
    UnkBlendEntries_ovb0000* v = reinterpret_cast<UnkBlendEntries_ovb0000*>(self);
    if (v->field_0x14 && v->count <= index) {
        u32 capacity = v->capacity;
        if (capacity <= index && v->step != 0)
            capacity = (index / v->step + 1) * v->step;
        fn_710000e610(self, capacity);
        u32 n = index + 1;
        fn_710000e610(self, n);
        for (u32 i = v->count; i < n; ++i)
            new (&v->data[i]) UnkBlendEntry_ovb0000();
        v->count = n;
    }
    return &v->data[index];
}

extern void* g_unk_7101a192a0[];

// The resource: a string id at +0x58 and the entries at +0x60, over the 0x58-byte base
// (fn_7100016f88).
struct UnkBlendSpaceRes_ovb0000 {
    void** vtable;
    u8 base_0x8[0x50];
    base::UnkStrIdMember name;  // 0x58
    UnkBlendEntries_ovb0000 entries;  // 0x60

    __attribute__((always_inline)) ~UnkBlendSpaceRes_ovb0000() {
        vtable = &g_unk_7101a192a0[2];
        entries.Free();
    }
};
static_assert(__builtin_offsetof(UnkBlendSpaceRes_ovb0000, entries) == 0x60, "layout");

void fn_710000b66c(UnkBlendSpaceRes_ovb0000* self) {
    self->~UnkBlendSpaceRes_ovb0000();
    fn_7100016f88(self);
}

// Erases the entry an index iterator names: the index goes through a 64-bit byte offset from the array and a pointer
// difference, which keeps the original's unfolded exact division.
void fn_710000e08c(UnkElem18Vector* self, const u32* it) {
    UnkBlendEntry_ovb0000* base = reinterpret_cast<UnkBlendEntry_ovb0000*>(self);
    u64 offset = (u64)*it * sizeof(UnkBlendEntry_ovb0000);
    UnkBlendEntry_ovb0000* p = reinterpret_cast<UnkBlendEntry_ovb0000*>(reinterpret_cast<char*>(base) + offset);
    fn_710000ee20(self, (u32)(p - base));
}
