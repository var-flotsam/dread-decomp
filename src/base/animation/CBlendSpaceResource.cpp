// base::animation::CBlendSpaceResource's unit (translation unit 14 of data/tu_map.csv), whose
// helpers also serve the reflected-object file readers (src/game/UnkObjectFileRead.cpp and its
// relatives): fn_710000ccd8 checks a file's version header against the registered one, and
// fn_7100010c60 parses a value of a given type into a scratch target and drops it (skipping it;
// the reader types are in base/reflection/UnkReflObjectRead.h). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

// The file read into memory and two words the opener sets (as in src/game/UnkObjectFileRead.cpp);
// +0x38 is the version check's result: 0 the registered version, 1 an older one, 2 a newer one.
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

// The registry of file versions, by the 64-bit id a file starts with.
struct UnkVersionRegistry;
struct UnkVersionEntry;
extern UnkVersionRegistry* g_unk_7101d4c8d0;
UnkVersionEntry* fn_71000a5118(UnkVersionRegistry* self, const u64* id);
// The entry's version (major, minor); false when it has none.
bool fn_71000e6ce0(UnkVersionEntry* self, u16* major, u16* minor);

// Reads the file's id and, when it is registered, its version; sets field_0x38 from the comparison
// (a file without an id is taken as current). True when the version is the registered one.
// NON_MATCHING(minor): on the early exit the store of field_0x38 and the setting of the return value
// are scheduled the other way round.
bool fn_710000ccd8(UnkFileReader* self) {
    u64 id = (u64)-1;
    fn_710007c714(&self->file, &id, 8, 1);
    if (id == (u64)-1) {
        self->field_0x38 = 0;
        return true;
    }
    UnkVersionEntry* entry = fn_71000a5118(g_unk_7101d4c8d0, &id);
    u16 major = 0;
    u16 minor = 0;
    if (entry && fn_71000e6ce0(entry, &major, &minor)) {
        u16 file_major = 0;
        u16 file_minor = 0;
        fn_710007c714(&self->file, &file_major, 2, 1);
        fn_710007c714(&self->file, &file_minor, 2, 1);
        u32 status;
        if (file_major == major && file_minor == minor) {
            status = 0;
        } else if (file_major > major) {
            status = 2;
        } else {
            status = 1;
            if (file_major >= major && file_minor >= minor)
                status = 2;
        }
        self->field_0x38 = status;
    }
    return self->field_0x38 == 0;
}

// The void type (src/base/reflection/UnkReflFunction.cpp).
extern base::reflection::CType g_unk_7101d5af48;

void fn_71000a2014(UnkReflValue* self);

// Parses a value of `type` (nothing for void) with the context's callbacks and file into a target of
// its own, and destroys what was read.
void fn_7100010c60(const UnkParseContext* ctx, const CType* type) {
    if (type == &g_unk_7101d5af48)
        return;
    UnkReflValue value;
    value.field_0x0 = nullptr;
    value.type = nullptr;
    value.kind = 2;
    value.field_0x14 = 1;
    fn_71000a2014(&value);
    UnkReadTarget target;
    fn_71000f0568(&target, &value, 5);
    target.field_0x5468 = 0;
    UnkParseContext sub(*ctx, &target);
    fn_710000f850(&sub, type, nullptr);
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_71000a1c60(&value);
}

// ---- fn_710000af7c: reading a blend space and resolving its animations ----

#include <string.h>

#include "base/UnkCountedResource.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkStrPair_0x10 {
    void* a;
    void* b;
};
void fn_710008d2ec(UnkStrPair_0x10* self, const base::global::CFilePathStrId* path);
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
// The parse (its result, nonzero on success, is not in the other files' declaration).
void fn_710000f750(UnkParseContext* self, void* root);
static inline s32 ParseRoot(UnkParseContext* ctx, void* root) {
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(ctx, root);
}
extern UnkParseCallbacks g_unk_7101d4cde8 __attribute__((visibility("hidden")));  // this unit's callbacks

// The resource registry and its lookup (as src/base/core/CAssetLink.cpp declares them).
struct UnkRegistry_7100d92158;
extern UnkRegistry_7100d92158* g_unk_7101d4bbc0;
struct UnkAssetResRef {
    UnkCountedResource* ptr;
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};
UnkAssetResRef fn_710002038c(UnkRegistry_7100d92158* registry, const UnkStrIdPathPair* ids, bool a, void* b);

// The categories an animation is looked up in: blend spaces (names ending in "mbls") and animations
// (src/base/animation/CBlendSpaceResource/fn_71000113b0.cpp and CAnimationResource/fn_7100005700.cpp
// register them).
extern char g_unk_7101d4c138[];
extern char g_unk_7101d4b418[];

extern "C" [[noreturn]] void fn_710000ce00();  // abort: a name too short for its suffix

// A blend space's animation: its path and the resource it resolves to.
struct UnkBlendSpaceAnim {
    u64 field_0x0;
    CFilePathStrId path;
    UnkCountedResource* resource;
};

struct UnkBlendSpace {
    virtual const CType* vf_0x0();  // its reflected type
    u8 pad_0x8[0x8];
    CFilePathStrId path;  // 0x10
    u8 pad_0x18[0x48];
    UnkBlendSpaceAnim* anims;  // 0x60
    u32 anim_count;
};

// The ids an animation is looked up by: its category and its path (counted copies).
struct UnkAnimIds {
    CStrId category;
    CFilePathStrId path;
};

// Reads the blend space from its file, then resolves each animation in the registry (keeping it only
// when it is an asset). False when the file could not be parsed.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
bool fn_710000af7c(UnkBlendSpace* self) {
    UnkFileReader reader;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, &self->path);
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
        fn_71000f0568(&target, &value, 5);
        fn_71000a1c60(&value);
    }
    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0) {
        UnkParseContext ctx(g_unk_7101d4cde8, &reader, &target);
        UnkReadEntry* end = target.entries + target.count;
        if (ParseRoot(&ctx, target.count ? end[-1].field_0x0->field_0x10 : nullptr)) {
            for (UnkBlendSpaceAnim* a = self->anims, *e = a + self->anim_count; a != e; ++a) {
                const char* name =
                    base::reflection::refl_RntStringOf(*reinterpret_cast<const CStrId*>(&a->path)).data;
                if (!name)
                    name = "";
                u64 len = strlen(name);
                if (len < 4)
                    fn_710000ce00();
                bool blend = __builtin_memcmp(name + len - 4, "mbls", 4) == 0;
                const CStrId& category =
                    *reinterpret_cast<const CStrId*>(blend ? g_unk_7101d4c138 : g_unk_7101d4b418);
                UnkAnimIds ids{category, a->path};
                UnkAssetResRef found =
                    fn_710002038c(g_unk_7101d4bbc0, reinterpret_cast<const UnkStrIdPathPair*>(&ids), true, nullptr);
                UnkCountedResource* res = found.ptr;
                if (res) {
                    auto* obj = reinterpret_cast<UnkBlendSpace*>(res);
                    if (!fn_710009d50c(obj->vf_0x0(), reinterpret_cast<const CType*>(fn_7100016894())))
                        res = nullptr;
                }
                if (a->resource != res) {
                    if (res)
                        ++res->ref_count;
                    if (UnkCountedResource* old = a->resource) {
                        if (--old->ref_count == 0)
                            fn_710001737c(old);
                    }
                    a->resource = res;
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
