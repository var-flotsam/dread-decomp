// From unit 2637 of tools/tu_map.py (0x7100fd1c7c-0x7100fdeaa4, the music manager and
// sound::CMusicManager::CTunableMusicManager by reflection; the constructor is
// src/game/sound/CMusicManager.cpp): reading the music manager's reflected object from its file.
// The file reader's types repeat src/game/sound/UnkMusicPresets.cpp's (the same names and
// layouts); names and layouts are placeholders.
// Holds fn_7100fd51a0.
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

// The id and path pair (base/UnkStrIdPathPair.h) the reader opens.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

// Builds the id and path pair from a file-path id.
void fn_710008d2ec(UnkStrPair_0x10* self, const base::global::CFilePathStrId* path);

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000ccd8(UnkFileReader* self);

// This unit's parser callbacks, the file's path and the object read with its type.
extern UnkParseCallbacks g_unk_71729f08d0 __attribute__((visibility("hidden")));
extern base::global::CFilePathStrId g_unk_71729f08a8 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_7172756e50;
extern u8 g_unk_7172757530[];

void fn_710000f750(UnkParseContext* self, void* root);

// Parses the file into the target, starting from its last entry's node.
static inline void Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71729f08d0, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Reads the music manager's tunables from their file.
void fn_7100fd51a0() {
    UnkFileReader reader;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, &g_unk_71729f08a8);
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
