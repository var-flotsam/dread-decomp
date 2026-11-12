// CPlaythroughDef's unit (tools/tu_map.py): fn_71002f2a94 reads the definition's reflected object
// from the file its path (+0x8) names, the same reader as the sound manager's resources
// (src/game/sound/CSoundManager/CSoundManager_tables_ovb0038.cpp) with this unit's parser callbacks
// (g_unk_71726ae5a8); fn_71002f500c is a reflected invoker answering, as a reflected bool, whether
// the string id its argument holds is in the id list (+0xa8, count at +0xb0) of the object at +8.
// Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU25.h"
#include "types.h"

using base::reflection::UnkReflObject;

// The id and path pair (base/UnkStrIdPathPair.h) the reader opens.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

// The id and path pair built from a path, destroyed out of line.
struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    ~UnkPathPairTemp_b45a1() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71726ae5a8 __attribute__((visibility("hidden")));

struct UnkPlaythroughDefFile_ovb0123 : UnkReflObject {
    u8 path[0x50];  // 0x8
};

struct UnkPlaythroughLoadRequest_ovb0123 {
    u8 unk_0x0[0x10];
    u32 mode;
};

static inline s32 Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71726ae5a8, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(
        &ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Reads the object from the file its path names; whether it parsed.
bool fn_71002f2a94(UnkPlaythroughDefFile_ovb0123* self, const UnkPlaythroughLoadRequest_ovb0123* request) {
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

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkPlaythroughIds_ovb0123 {
    u8 pad_0x0[0xa8];
    CStrId* ids;  // 0xa8
    u32 count;    // 0xb0
};

static inline void* ArgPtr(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

static inline const CStrId* Find(const UnkPlaythroughIds_ovb0123* self, const CStrId& id) {
    for (u32 i = 0; i < self->count; ++i)
        if (self->ids[i].data == id.data)
            return &self->ids[i];
    return &self->ids[self->count];
}

UnkReflValue fn_71002f500c(void*, char* obj, UnkReflValue* const* args) {
    const UnkPlaythroughIds_ovb0123* self = *reinterpret_cast<UnkPlaythroughIds_ovb0123**>(obj + 8);
    CStrId id(*static_cast<CStrId*>(ArgPtr(args[0])));
    bool value = Find(self, id) != self->ids + self->count;
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
