// Unit 1728 (no reliable name): string ids read from a file, a vector growth and a destructor. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "types.h"
#include <new>
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "game_s2/UnkOwnedList.h"

using base::global::CFilePathStrId;
using base::global::CRntFile;
using base::global::UnkStringData;

u64 fn_7100b89f84(CRntFile* file, CFilePathStrId* out) {
    u16 length;
    u64 total = fn_710007c714(file, &length, 2, 1);
    if (length != 0) {
        char buffer[0x80];
        u64 read = fn_710007c714(file, buffer, length, 1);
        u64 size = length;
        buffer[size] = 0;
        u64 hash = fn_7100001570(buffer, size);
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        UnkStringData* data = fn_71000804f8(g_unk_7101d47200, hash);
        if (data)
            ++data->ref_count;
        else
            data = fn_71000801e4(g_unk_7101d47200, hash, buffer, size, true);
        total += read;
        base::global::UnkFilePathStrIdBase raw;
        raw.data = data;
        *out = static_cast<CFilePathStrId&&>(reinterpret_cast<CFilePathStrId&>(raw));
    }
    return total;
}

using base::global::CStrId;
using base::global::CRntFile;
using base::global::UnkStringData;

u64 fn_7100b8a428(CRntFile* file, CStrId* out) {
    u16 length;
    u64 total = fn_710007c714(file, &length, 2, 1);
    if (length != 0) {
        char buffer[0x80];
        u64 read = fn_710007c714(file, buffer, length, 1);
        u64 size = length;
        buffer[size] = 0;
        u64 hash = fn_7100001570(buffer, size);
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        UnkStringData* data = fn_71000804f8(g_unk_7101d47108, hash);
        if (data)
            ++data->ref_count;
        else
            data = fn_71000801e4(g_unk_7101d47108, hash, buffer, size, true);
        total += read;
        struct {
            UnkStringData* data;
        } raw;
        raw.data = data;
        out->ReleaseAndTake(reinterpret_cast<CStrId&>(raw));
    }
    return total;
}

namespace {

// A growable array's fields, moved by taking its settings and storage.
struct UnkRawVec_b0246 {
    u8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkRawVec_b0246(UnkRawVec_b0246&& other) {
        data = other.data;
        count = other.count;
        capacity = other.capacity;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkRawVec_b0246() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkCountedRef_b0246 {
    UnkCountedResource* ptr;

    UnkCountedRef_b0246(UnkCountedRef_b0246&& other) {
        ptr = nullptr;
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
    ~UnkCountedRef_b0246() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

struct UnkElem_b0246 {
    u64 field_0x0;
    UnkRawVec_b0246 first;   // 0x8
    UnkRawVec_b0246 second;  // 0x30
    UnkCountedRef_b0246 ref;  // 0x58

    UnkElem_b0246(UnkElem_b0246&& other)
        : field_0x0(other.field_0x0), first(static_cast<UnkRawVec_b0246&&>(other.first)),
          second(static_cast<UnkRawVec_b0246&&>(other.second)), ref(static_cast<UnkCountedRef_b0246&&>(other.ref)) {}
};
static_assert(sizeof(UnkElem_b0246) == 0x60, "UnkElem_b0246 layout");

}  // namespace

void fn_7100b8a5d8(base::UnkVector<u8>* vector, u32 capacity) {
    using T = UnkElem_b0246;
    auto* self = reinterpret_cast<base::UnkVector<T>*>(vector);
    if (self->capacity < capacity) {
        auto* p = static_cast<T*>(self->allocator->vf_0x40((u64)capacity * sizeof(T), 8, self->flags));
        if (self->data) {
            T* src = self->data;
            T* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) T(static_cast<T&&>(*src));
                src->~T();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = capacity;
    }
}

void fn_7100b8aa94(void* part);
void fn_7100c95a68(void* element);

struct UnkElem28_b0246 {
    u8 unk_0x0[0x28];
    ~UnkElem28_b0246() { fn_7100c95a68(this); }
};

struct UnkTimelineThing_b0246 {
    base::global::CStrId name;  // 0x0
    base::UnkVector<UnkElem28_b0246> elements;  // 0x8
    u8 pad_0x30[0xc8];
    UnkOwnedList owned;  // 0xf8
};

void fn_7100b8b7e8(UnkTimelineThing_b0246* self) {
    self->owned.~UnkOwnedList();
    fn_7100b8aa94(reinterpret_cast<u8*>(self) + 0x30);
    self->elements.Free();
    self->name.Reset();
}
