// Unit 2181 (tools/tu_map.py): reads a record from a file: two words, a file path, two lists of
// word pairs, then resolves the path to a curve resource and keeps it. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/curves/CCurveResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU27.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntFile;
using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::CType;

u64 fn_7100b89f84(CRntFile* file, CFilePathStrId* out);

// A pair of words read from the file; a new one is unset (all bits set).
struct UnkId_lc7ab58 {
    u32 field_0x0;
    u32 field_0x4;
    UnkId_lc7ab58() : field_0x0(~0u), field_0x4(~0u) {}
    // Declared, so the arrays move their elements one by one.
    ~UnkId_lc7ab58() {}
};

// The resource found for a path, released when it goes out of scope.
struct UnkAssetResRef {
    UnkCountedResource* ptr;
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// A resource seen as a reflected object: slot 0 is its class.
struct UnkReflResource_lc7ab58 {
    virtual const CType* vf_0x0();
};

// The ids a resource is looked up by: its category and its path (counted copies).
struct UnkResourceIds_lc7ab58 {
    CStrId category;
    CFilePathStrId path;
};

struct UnkRecord_lc7ab58 {
    u32 field_0x0;
    u32 field_0x4;
    CRntVector<UnkId_lc7ab58> field_0x8;
    CRntVector<UnkId_lc7ab58> field_0x30;
    UnkCountedResource* curve;  // 0x58
};
static_assert(__builtin_offsetof(UnkRecord_lc7ab58, curve) == 0x58, "layout");

// Appends an unset id and returns it.
static inline UnkId_lc7ab58& AddId(CRntVector<UnkId_lc7ab58>* ids) {
    u32 n = ids->count + 1;
    ids->Reserve(n);
    for (u32 i = ids->count; i < n; ++i)
        ids->data[i] = UnkId_lc7ab58();
    ids->count = n;
    UnkId_lc7ab58* end = &ids->data[n];
    return n ? end[-1] : *end;
}

void fn_7100c7ab58(UnkRecord_lc7ab58* self, CRntFile* file) {
    fn_710007c714(file, &self->field_0x0, sizeof(u32), 1);
    fn_710007c714(file, &self->field_0x4, sizeof(u32), 1);
    CFilePathStrId path;
    fn_7100b89f84(file, &path);
    u32 count;
    fn_710007c714(file, &count, sizeof(count), 1);
    if (count) {
        self->field_0x8.Reserve(count);
        for (u32 i = 0; i < count; ++i)
            fn_710007c714(file, &AddId(&self->field_0x8), sizeof(UnkId_lc7ab58), 1);
    }
    u32 count2;
    fn_710007c714(file, &count2, sizeof(count2), 1);
    if (count2) {
        self->field_0x30.Reserve(count2);
        for (u32 i = 0; i < count2; ++i)
            fn_710007c714(file, &AddId(&self->field_0x30), sizeof(UnkId_lc7ab58), 1);
    }

    UnkRegistry_7100d92158* registry = g_unk_7101d4bbc0;
    UnkResourceIds_lc7ab58 ids{*reinterpret_cast<const CStrId*>(g_unk_7101d4f338), path};
    UnkAssetResRef found =
        fn_710002038c(registry, reinterpret_cast<const UnkStrIdPathPair*>(&ids), true, nullptr);
    // The resource, kept (counted) only when it is a curve.
    UnkAssetResRef kept{nullptr};
    if (UnkCountedResource* r = found.ptr) {
        auto* obj = reinterpret_cast<UnkReflResource_lc7ab58*>(r);
        if (fn_710009d50c(obj->vf_0x0(), reinterpret_cast<const CType*>(fn_7100029a50()))) {
            ++r->ref_count;
            kept.ptr = r;
        }
    }
    UnkCountedResource* res = kept.ptr;
    if (self->curve != res) {
        if (res)
            ++res->ref_count;
        if (UnkCountedResource* old = self->curve) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        self->curve = res;
    }
}
