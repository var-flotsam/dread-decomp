// From unit 2602 of tools/tu_map.py (0x7100f87e50-0x7100f8ae78, initialiser fn_7100f8acd0, no
// class hint): the reflected class with the preset dictionary (dicPresets, CreateNewPreset,
// AfterPresetDeserialization; src/reflection/registration_02.cpp) and two maps of presets, whose
// changes are passed on to the music manager (game/sound/CMusicManager.h) through fn_7100fd5348.
// The file reader's types repeat src/game/UnkObjectFileLoader.cpp's (the same names and
// layouts); names and layouts are placeholders.
// Holds fn_7100f89450 (reads the reflected global object from a file the path registry knows) and
// fn_7100f89678 (empties both maps).
#include "base/UnkDestructors.h"
#include "base/UnkHashStream.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

// The music manager (game/sound/CMusicManager.h; 0xf18 bytes, built by fn_7100fd3804) and what
// this unit asks of it.
extern void* g_unk_71726bc560;
void fn_7100fd5348(void* manager);

static inline void* MusicManager() {
    if (!g_unk_71726bc560) {
        void* m = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(m);
        g_unk_71726bc560 = m;
    }
    return g_unk_71726bc560;
}

// A string (base::global::CRntString's layout), as src/game/UnkObjectFileLoader.cpp names it.
struct UnkEntry_0x30 {
    u8 data[0x30];
};

// The id and path pair (base/UnkStrIdPathPair.h) that fn_710008d4c0 builds and fn_710008d8e8
// destroys.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

void fn_710008d4c0(UnkStrPair_0x10* s, const UnkEntry_0x30* e);
bool fn_7100087850(UnkRegistry_0x70* r, const UnkStrPair_0x10* s);
extern UnkRegistry_0x70* g_unk_7101d4b5a0;

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000ccd8(UnkFileReader* self);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_71729ea458 __attribute__((visibility("hidden")));

void fn_710000f750(UnkParseContext* self, void* root);

// The object read and its type.
extern base::reflection::CType g_unk_7172756e50;
extern u8 g_unk_7172757530[];

// Parses the file into the target, starting from its last entry's node.
static inline void Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71729ea458, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Tells the music manager first, then reads the file at `path` when the registry knows it.
void fn_7100f89450(void*, const UnkEntry_0x30* path) {
    fn_7100fd5348(MusicManager());
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

// The values of the two maps (out-of-line destructors).
void fn_7100d90304(void* value);
void fn_7100fee034(void* value);

// The maps' key: a string id released in two steps (base/UnkDestructors.h).
using UnkPresetKey = base::UnkStrIdMember;

// A chained map keyed by string id (base/UnkChainedMap.h's layout), whose nodes hold the key, the
// value at +8 and the next node at +Next. The nodes are walked from +0x40.
template <u32 Next, void (*DestroyValue)(void*)>
struct UnkPresetMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    base::UnkVector<char*> buckets;
    char* field_0x38;
    char* field_0x40;

    // Destroys every node and empties the buckets (keeping them).
    void Clear() {
        if (count) {
            char* node = field_0x40;
            while (node) {
                char* next = *reinterpret_cast<char**>(node + Next);
                DestroyValue(node + 8);
                reinterpret_cast<UnkPresetKey*>(node)->~UnkPresetKey();
                allocator->vf_0x48(node);
                node = next;
            }
            __builtin_memset(buckets.data, 0, buckets.capacity * 8ul);
            count = 0;
            field_0x38 = nullptr;
            field_0x40 = nullptr;
        }
    }
};

struct UnkMusicPresets {
    UnkPresetMap<0x70, fn_7100d90304> field_0x0;
    UnkPresetMap<0x58, fn_7100fee034> field_0x48;
};
static_assert(sizeof(UnkMusicPresets) == 0x90, "UnkMusicPresets placeholder layout");

// Empties both maps and tells the music manager.
void fn_7100f89678(UnkMusicPresets* self) {
    self->field_0x0.Clear();
    self->field_0x48.Clear();
    fn_7100fd5348(MusicManager());
}
