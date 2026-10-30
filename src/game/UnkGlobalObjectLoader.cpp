// Translation unit 2608 of data/tu_map.csv (0x7100f9b32c-0x7100f9cde8, no class hint): loads a
// reflected global object from a file when the path registry knows the path (fn_7100f9bf8c), as
// fn_7100f947b0 in src/game/UnkObjectFileLoader.cpp does with another parser table
// (g_unk_71729ec650). The file reader's types repeat that file's (the same names and layouts);
// names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

// A string (base::global::CRntString's layout), as src/game_hi/fn_71011b69d8.cpp names it.
struct UnkEntry_0x30 {
    u8 data[0x30];
};

// The id and path pair (base/UnkStrIdPathPair.h) that fn_710008d4c0 builds and fn_710008d8e8
// destroys, as src/game_hi/fn_71011b69d8.cpp names it.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

void fn_710008d4c0(UnkStrPair_0x10* s, const UnkEntry_0x30* e);
extern UnkRegistry_0x70* g_unk_7101d4b5a0;

// The file read into memory: the in-memory file (constructed by fn_710007be8c, destroyed by
// fn_710007c0e4; base/UnkHashStream.h) and two words the opener sets (+0x38 is zero after a
// successful read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000ccd8(UnkFileReader* self);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71729ec650 __attribute__((visibility("hidden")));

void fn_710000f750(UnkParseContext* self, void* root);

// The object loaded and its type.
extern base::reflection::CType g_unk_7172756e50;
extern u8 g_unk_7172757530[];

// Parses the file into the target, starting from its last entry's node.
static inline void Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71729ec650, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

void fn_7100f9bf8c(void* self, const UnkEntry_0x30* path) {
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    bool known;
    {
        UnkStrPair_0x10 s;
        fn_710008d4c0(&s, path);
        known = fn_7100087850(reg, &s);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (!known)
        return;

    UnkFileReader reader;
    {
        UnkStrPair_0x10 s;
        fn_710008d4c0(&s, path);
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
