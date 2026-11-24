// navmesh::CDynamicSmartLinkGroup's unit (tools/tu_map.py unit 351): loads the group. Takes its path,
// reads its reflected object from the file (with this unit's parser callbacks), then resolves each link
// whose path ends in ".bmslink" in the resource registry. Whether the file parsed. Names and layouts
// are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkFileReader.h"
#include "base/UnkHashStream.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkPathPairTemp_b45a1.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU27.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkAssetResRef {
    UnkCountedResource* ptr;
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71726a2a30 __attribute__((visibility("hidden")));

// A link: its file path and the resource it resolves to.
struct UnkSmartLink_l265e24 {
    CFilePathStrId path;
    UnkCountedResource* resource;
};

struct UnkSmartLinkGroup_l265e24 {
    virtual const CType* vf_0x0();  // its reflected type
    u8 field_0x8[0x8];
    CFilePathStrId field_0x10;
    u8 pad_0x18[0x40];
    CFilePathStrId path;            // 0x58
    UnkSmartLink_l265e24* links;    // 0x60
    u32 link_count;
};

// The ids a link is looked up by: its category and its path (counted copies).
struct UnkSmartLinkIds_l265e24 {
    CStrId category;
    CFilePathStrId path;
};

// Parses the file into the target, starting from its last entry's node; whether it succeeded.
static inline bool Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71726a2a30, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr) != 0;
}

// Whether a link's path names a smart link file (reading the path's text counts as a read).
static inline bool IsSmartLinkFile(const CFilePathStrId& path) {
    ++path.data->reads;
    return path.data != &g_unk_7101d47200->empty && CRntString_Contains(&path.data->String(), ".bmslink");
}

bool CDynamicSmartLinkGroup_LoadFromFileAndResolveLinks(UnkSmartLinkGroup_l265e24* self) {
    {
        UnkPathPairTemp_b45a1 s = MakePathPairFromRegistryCallback(self->field_0x8);
        self->path = *reinterpret_cast<const CFilePathStrId*>(&s.path);
    }
    UnkFileReader reader;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, &self->field_0x10);
        fn_710007be8c(&reader.file, 0x16, nullptr);
        reader.field_0x38 = 3;
        reader.field_0x3c = 2;
        bool opened;
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(&reader, &s, &mode);
        }
        if (opened)
            fn_710000ccd8(&reader);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    UnkReadTarget target;
    {
        const CType* type = self->vf_0x0();
        UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, self, true);
        fn_71000f0568(&target, &value, 4);
        fn_71000a1c60(&value);
    }
    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0) {
        if (Parse(&reader, &target)) {
            for (UnkSmartLink_l265e24 *l = self->links, *e = l + self->link_count; l != e; ++l) {
                if (!IsSmartLinkFile(l->path))
                    continue;
                UnkRegistry_7100d92158* registry = g_unk_7101d4bbc0;
                UnkSmartLinkIds_l265e24 ids{reinterpret_cast<const CStrId&>(g_unk_71727b98b0), l->path};
                UnkAssetResRef found =
                    MakeResourceFromRegistryFactory(registry, reinterpret_cast<const UnkStrIdPathPair*>(&ids), true, nullptr);
                UnkCountedResource* res = found.ptr;
                if (l->resource != res) {
                    if (res)
                        ++res->ref_count;
                    if (UnkCountedResource* old = l->resource) {
                        if (--old->ref_count == 0)
                            fn_710001737c(old);
                    }
                    l->resource = res;
                }
            }
            ok = true;
        }
    }
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
