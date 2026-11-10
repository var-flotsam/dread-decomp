// Unit 2762 (tools/tu_map.py): opens the file at a path for reading and keeps its contents: the data
// pointer at +0x68 and the range of ids its first two halfwords give (+0x70 the first, +0x72 how many).
// Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntString.h"
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
void fn_710007c144(UnkHashStream_0x38*);

// The file read into memory: the in-memory file and two words the opener sets.
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);

struct UnkIdRange_ovo0318 {
    u16 first;
    u16 last;
};

struct UnkIdRangeHolder_ovo0318 {
    u8 pad_0x0[0x30];
    base::global::CRntFile file;  // 0x30
    const UnkIdRange_ovo0318* data;  // 0x68
    u16 first;                     // 0x70
    u16 count;                     // 0x72
};

void fn_710120269c(UnkIdRangeHolder_ovo0318* self, const UnkEntry_0x30* path) {
    fn_710007c144(&self->file);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d4c0(&s, path);
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&self->file), &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (opened) {
        const UnkIdRange_ovo0318* range = reinterpret_cast<const UnkIdRange_ovo0318*>(self->file.data);
        fn_710007c1a4(&self->file);
        u16 first = range->first;
        u16 count = range->last - first + 1;
        self->data = range;
        self->first = first;
        self->count = count;
    }
}
