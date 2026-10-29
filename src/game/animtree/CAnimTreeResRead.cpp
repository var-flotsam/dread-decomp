// From unit 405 of tools/tu_map.py (0x71002dfd50-0x71002e04c0, animtree::CAnimTreeRes by
// reflection; its static initialiser is src/game/animtree/CAnimTreeRes/fn_71002e0370.cpp): reading an
// animation tree resource from its file. The file reader's types repeat
// src/base/animation/CBlendSpaceResource.cpp's; names and layouts are placeholders.
// Holds fn_71002e0090.
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

struct UnkStrPair_0x10 {
    void* a;
    void* b;
};
void fn_710008d2ec(UnkStrPair_0x10* self, const base::global::CFilePathStrId* path);
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000ccd8(UnkFileReader* self);
// The parse (its result, nonzero on success, is not in the other files' declaration).
void fn_710000f750(UnkParseContext* self, void* root);
static inline s32 ParseRoot(UnkParseContext* ctx, void* root) {
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(ctx, root);
}
extern UnkParseCallbacks g_unk_71726aade8 __attribute__((visibility("hidden")));  // this unit's callbacks

struct UnkAnimTreeRes {
    virtual const base::reflection::CType* vf_0x0();  // its reflected type
    u8 pad_0x8[0x8];
    base::global::CFilePathStrId path;  // 0x10
};

// Reads the resource from its file; whether the file could be parsed.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
bool fn_71002e0090(UnkAnimTreeRes* self) {
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
        const base::reflection::CType* type = self->vf_0x0();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, self, true);
        fn_71000f0568(&target, &value, 0x41);
        fn_71000a1c60(&value);
    }
    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0) {
        UnkParseContext ctx(g_unk_71726aade8, &reader, &target);
        UnkReadEntry* end = target.entries + target.count;
        ok = ParseRoot(&ctx, target.count ? end[-1].field_0x0->field_0x10 : nullptr) != 0;
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
