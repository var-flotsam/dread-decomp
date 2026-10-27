// From unit 2660 of tools/tu_map.py (0x71010621bc-0x71010626f0, initialiser fn_7101062570; its
// other functions are a reflected class's helpers): reads the reflected global object that
// src/game/UnkObjectFileLoader.cpp loads (g_unk_7172757530, of type g_unk_7172756e50) from a file,
// with this unit's parser callbacks and without asking the path registry first. The types below
// repeat that file's (the same names and layouts; the reader types are in
// base/reflection/UnkReflObjectRead.h); names and layouts are placeholders.
// Holds fn_7101062398.
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

// The id and path pair (base/UnkStrIdPathPair.h) the caller passes.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path,
                   const base::global::CRntString* mode);
void fn_710000ccd8(UnkFileReader* self);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_7172a000d0 __attribute__((visibility("hidden")));

void fn_710000f750(UnkParseContext* self, void* root);

// The object read and its type.
extern base::reflection::CType g_unk_7172756e50;
extern u8 g_unk_7172757530[];

// Parses the file into the target, starting from its last entry's node.
static inline void Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_7172a000d0, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

void fn_7101062398(void*, void*, const UnkStrPair_0x10* path) {
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    reader.field_0x38 = 3;
    reader.field_0x3c = 2;
    bool opened;
    {
        base::global::CRntString mode("rb");
        opened = fn_710007c1ac(&reader, path, &mode);
    }
    if (opened)
        fn_710000ccd8(&reader);
    UnkReadTarget target;
    {
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, &g_unk_7172756e50, g_unk_7172757530, true);
        fn_71000f0568(&target, &value, 5);
        fn_71000a1c60(&value);
    }
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        Parse(&reader, &target);
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
}
