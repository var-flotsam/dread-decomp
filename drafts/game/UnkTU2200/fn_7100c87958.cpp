// Unit 2200 (CCutSceneDef neighbourhood): fn_7100c87958 names each 0xa0-byte entry after its file path:
// the file's name without its extension, or, for a file whose name starts with "actor", the name of the
// folder it is in. Names and layouts are placeholders.
#include <string.h>

#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/game/UnkTU2553/fn_7100de9c74.cpp.
struct UnkInlineString_lc87958 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_lc87958() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        field_0x138 = 0;
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x140 = 0;
    }
    ~UnkInlineString_lc87958() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// Room for a string id that the out-of-line constructor (fn_7100000850) builds; it has no destructor,
// since its string is always handed on.
union UnkStrIdSlot_lc87958 {
    CStrId id;
    UnkStrIdSlot_lc87958() {}
    ~UnkStrIdSlot_lc87958() {}
};

struct UnkCutSceneEntry_lc87958 {
    u8 pad_0x0[0x8];
    UnkStringData* path;  // a file-path id
    CStrId name;
    u8 pad_0x18[0x88];
};
static_assert(sizeof(UnkCutSceneEntry_lc87958) == 0xa0, "layout");

struct UnkCutSceneOwner_lc87958 {
    u8 pad_0x0[0x90];
    UnkCutSceneEntry_lc87958* entries;
    u32 count;
};

void fn_7100c87958(UnkCutSceneOwner_lc87958* self) {
    UnkCutSceneEntry_lc87958* end_entry = self->entries + self->count;
    for (UnkCutSceneEntry_lc87958* e = self->entries; e != end_entry; ++e) {
        ++e->path->reads;
        const CRntString& file = e->path->String();
        UnkInlineString_lc87958 name;
        fn_710007eaa0(name.Str(), &file);
        s32 dot = fn_710007fb10(name.Str(), '.', -1);
        s32 slash = fn_710007fb10(name.Str(), '/', -1);
        char* data = name.Str()->data;
        const char* text = data ? data : g_unk_710162e2ed;
        u32 start;
        u32 end;
        if (strncmp(text + slash + 1, "actor", 5) != 0) {
            // The file's name, up to its extension.
            start = slash + 1;
            end = dot == -1 ? name.Str()->length : dot;
        } else {
            // The name of the folder the file is in.
            s64 i = slash;
            while (i > 0 && data[i - 1] != '/')
                --i;
            start = i;
            end = slash == -1 ? name.Str()->length : slash;
        }
        {
            UnkInlineString_lc87958 part;
            fn_710007e7f8(part.Str(), data + start, end - start);
            fn_710007eaa0(name.Str(), part.Str());
        }
        UnkStrIdSlot_lc87958 slot;
        fn_7100000850(&slot.id, *name.Str(), true);
        e->name = static_cast<CStrId&&>(slot.id);
    }
}
