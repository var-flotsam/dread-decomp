// CPlaythrough's unit (tools/tu_map.py unit 424): reads the object's reflected fields from the file its
// path (+0x8) names, with this unit's parser callbacks (g_unk_71726ad6b8); whether it parsed.
// Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkPathPairTemp_b45a1.h"
#include "game/UnkTU25.h"
#include "game/UnkTU56.h"
#include "types.h"
#include "base/UnkStrPair.h"
#include "base/UnkFileReader.h"

using base::reflection::UnkReflObject;

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71726ad6b8 __attribute__((visibility("hidden")));

struct UnkPlaythroughFile_l2ec894 : UnkReflObject {
    u8 path[0x50];  // 0x8
};

struct UnkPlaythroughLoadRequest_l2ec894 {
    u8 unk_0x0[0x10];
    u32 mode;
};

static inline s32 Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71726ad6b8, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(
        &ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Reads the object from the file its path names; whether it parsed.
bool CPlaythrough_ReadReflectedFieldsFromFile(UnkPlaythroughFile_l2ec894* self, const UnkPlaythroughLoadRequest_l2ec894* request) {
    UnkFileReader reader;
    {
        UnkPathPairTemp_b45a1 s = MakePathPairFromRegistryCallback(self->path);
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
