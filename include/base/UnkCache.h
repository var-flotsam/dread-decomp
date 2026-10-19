#pragma once

// Adding an entry to a cache keyed by a pair of string ids (a CStrId, then a CFilePathStrId):
// empty ids are rejected; the object made by the owner's slot 0x40 is loaded through
// fn_71000171a0 with a request object (freed through fn_7100019fc0 on failure), and a node
// (the path id, the object) is linked into the bucket chosen by the path id's hash. Layout and
// names are placeholders (fn_7100020b48 and its copies).

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace base {

struct UnkLoadRequest {
    char* vtable;
    u64 field_0x8;
    u32 field_0x10;
    void* field_0x18;
};

struct UnkCacheOwner {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* vf_0x40(const char* key);
};

template <class Tag>
struct UnkCacheTraits;  // bucket(buckets, i): the address of bucket i

}  // namespace base

extern char g_unk_7101a19610[];
bool fn_71000171a0(void* obj, const char* key, base::UnkLoadRequest* request);
void fn_7100019fc0(void* obj, bool flag);

namespace base {

template <class Tag>
void* refl_CacheAdd(UnkCacheOwner* self, const char* key, u32 a, void* b) {
    using global::UnkStringData;
    char* s = reinterpret_cast<char*>(self);
    if (*reinterpret_cast<UnkStringData* const*>(key) == &g_unk_7101d47108->empty ||
        *reinterpret_cast<UnkStringData* const*>(key + 8) == &g_unk_7101d47200->empty)
        return nullptr;
    void* obj = self->vf_0x40(key);
    UnkLoadRequest request;
    request.vtable = g_unk_7101a19610 + 0x10;
    request.field_0x8 = 0;
    request.field_0x10 = a;
    request.field_0x18 = b;
    if (fn_71000171a0(obj, key, &request)) {
        ++*reinterpret_cast<u32*>(s + 0x50);
        char* node = static_cast<char*>(
            (*reinterpret_cast<UnkAllocator**>(s + 0x40))->vf_0x40(0x18, 8, *reinterpret_cast<u32*>(s + 0x48)));
        if (node) {
            UnkStringData* d = *reinterpret_cast<UnkStringData* const*>(key + 8);
            *reinterpret_cast<UnkStringData**>(node) = d;
            if (&g_unk_7101d47200->empty != d)
                ++d->ref_count;
            *reinterpret_cast<void**>(node + 8) = obj;
            *reinterpret_cast<char**>(node + 0x10) = nullptr;
        }
        u32 i;
        if (*reinterpret_cast<u32*>(s + 0x10)) {
            const char* d = *reinterpret_cast<char* const*>(key + 8);
            u32 h = *reinterpret_cast<const u32*>(d + 0x30) ^ *reinterpret_cast<const u32*>(d + 0x34);
            i = h % *reinterpret_cast<u32*>(s + 0x14);
        } else {
            i = ~0u;
        }
        u32 first = *reinterpret_cast<u32*>(s + 0x30);
        *reinterpret_cast<u32*>(s + 0x30) = i < first ? i : first;
        if (i <= first)
            *reinterpret_cast<char**>(s + 0x38) = node;
        char** slot = UnkCacheTraits<Tag>::bucket(s + 8, i);
        *reinterpret_cast<char**>(node + 0x10) = *slot;
        *slot = node;
        ++*reinterpret_cast<u32*>(s + 0x4c);
    } else {
        fn_7100019fc0(obj, true);
        obj = nullptr;
    }
    return obj;
}

}  // namespace base
