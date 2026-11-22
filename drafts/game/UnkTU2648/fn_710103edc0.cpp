// Unit 2648: the string id of a preset: the part of the file path after "/presets/", in lower case and with
// each '/' turned into '#'; the empty id when the name or the path is empty or the path has no "/presets/".
// Names and layouts are placeholders.
#include <string.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringData;

using UnkPresetText_l03edc0 = UnkBufferString<0x100, g_unk_7101a194b8>;

struct UnkPresetSource_l03edc0 {
    CStrId name;
    CFilePathStrId path;  // 0x8
};

// A string id, returned in memory.
struct UnkPresetId_l03edc0 {
    UnkStringData* data;

    explicit UnkPresetId_l03edc0(const CRntString& text) {
        fn_7100000850(reinterpret_cast<UnkStrIdBase*>(this), text, true);
    }
    UnkPresetId_l03edc0(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkPresetId_l03edc0();
};

// The text of a file-path id, counting the read.
static inline const CRntString* Text(const CFilePathStrId& id) {
    ++id.data->reads;
    return &id.data->String();
}

// Copies `src` into `dst` with A-Z turned into a-z.
static inline void CopyLower(char* dst, const char* src) {
    dst[0] = 0;
    u32 length = static_cast<u32>(strlen(src));
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            if (static_cast<u32>(c - 'A') < 26)
                c += 0x20;
            dst[i] = c;
        }
        dst[length] = 0;
    }
}

UnkPresetId_l03edc0 fn_710103edc0(const UnkPresetSource_l03edc0* self) {
    if (self->name.data == &g_unk_7101d47108->empty || self->path.data == &g_unk_7101d47200->empty)
        return g_unk_7101d470e0;
    char lower[0x200];
    const CRntString* text = Text(self->path);
    UnkPresetText_l03edc0 path;
    CRntString_Assign(path.Str(), text);
    const char* prefix = "/presets/";
    static size_t s_prefix_length = strlen(prefix);
    s32 found = CRntString_Find(path.Str(), prefix, 0);
    if (found == 0)
        return g_unk_7101d470e0;
    u32 start = found + static_cast<u32>(s_prefix_length);
    UnkPresetText_l03edc0 rest;
    CRntString_AssignPrefix(rest.Str(), path.Str()->data + start, path.Str()->length - start);
    CopyLower(lower, rest.Str()->data);
    UnkPresetText_l03edc0 lowered(lower);
    u32 length = lowered.Str()->length;
    lower[length] = 0;
    for (u32 i = 0; i < length; ++i) {
        char c = lowered.Str()->data[i];
        lower[i] = c == '/' ? '#' : c;
    }
    UnkPresetText_l03edc0 id_text(lower);
    return UnkPresetId_l03edc0(*id_text.Str());
}
