// From unit 2637 of tools/tu_map.py (0x7100fd1c7c-0x7100fdeaa4, the music manager and
// sound::CMusicManager::CTunableMusicManager by reflection; the constructor is
// src/game/sound/CMusicManager.cpp): reading the music manager's reflected object from its file.
// The file reader's types repeat src/game/sound/UnkMusicPresets.cpp's (the same names and
// layouts); names and layouts are placeholders.
// Holds fn_7100fd51a0 and fn_7100fd6f74 (whether a music request is already playing).
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

// ---- fn_7100fd6f74: whether a music request is already playing ----

#include <new>

#include "base/global/CStrId.h"
#include "game/sound/CMusicManager.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// A track of a music request: its file and two words (0x18 bytes).
struct UnkMusicFile {
    CFilePathStrId path;
    u64 field_0x8;
    u64 field_0x10;
};

// The files, released before the request's name.
struct UnkMusicFiles : base::UnkVector<UnkMusicFile> {
    explicit UnkMusicFiles(u32 flags) : base::UnkVector<UnkMusicFile>(flags) {}
    ~UnkMusicFiles() { Free(); }
};

// A music request (0x60 bytes; the manager's request array at +0xb8 holds the playing ones): the
// channel it plays on, volumes, its files and its name.
struct UnkMusicRequest {
    u32 channel = 0;
    u32 field_0x4 = 1;
    f32 field_0x8 = 1.0f;
    u32 field_0xc = 0;
    u32 field_0x10 = 0;
    f32 field_0x14 = 1.0f;
    u8 field_0x18[6] = {};
    u16 field_0x1e = 0;
    u8 field_0x20[6] = {};
    bool field_0x26 = true;
    UnkMusicFiles files{0xf};  // 0x28
    u64 field_0x50 = 0;
    CStrId name{"", true};  // 0x58
};
static_assert(sizeof(UnkMusicRequest) == 0x60, "UnkMusicRequest placeholder layout");

// Fills the two requests a music id stands for.
void fn_7100fd73a4(UnkMusicManager* self, const CStrId* id, UnkMusicRequest* first, UnkMusicRequest* second);

// Whether the music `name` stands for is already what plays (on the channels in `mask`).
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
bool fn_7100fd6f74(UnkMusicManager* self, const base::global::CRntString* name, const u32* mask) {
    UnkMusicRequest first;
    UnkMusicRequest second;
    {
        union UnkBuiltId {
            CStrId id;
            UnkBuiltId() {}
            ~UnkBuiltId() { id.~CStrId(); }
        } built;
        fn_7100000850(&built.id, *name, true);
        fn_7100fd73a4(self, &built.id, &first, &second);
    }
    if (first.files.count == 0 && second.files.count == 0)
        return false;
    const UnkMusicRequest* playing = reinterpret_cast<const UnkMusicRequest*>(self->entries.data);
    if (first.name.data != g_unk_7101d470e0.data && first.name.data == playing[0].name.data)
        return true;
    bool result = false;
    if (first.files.count != 0 && playing[0].files.count != 0) {
        if (first.files.data[0].path.data == &g_unk_7101d47200->empty || !(*mask & (1u << (playing[0].channel & 0x1f))) ||
            playing[0].files.data[0].path.data != first.files.data[0].path.data)
            return false;
        result = true;
    }
    if (second.name.data != g_unk_7101d470e0.data && second.name.data == playing[1].name.data)
        return true;
    if (second.files.count == 0 || playing[1].files.count == 0)
        return result;
    if (second.files.data[0].path.data == &g_unk_7101d47200->empty ||
        !(*mask & (1u << (playing[1].channel & 0x1f))) || playing[1].files.data[0].path.data != second.files.data[0].path.data)
        return false;
    return true;
}
