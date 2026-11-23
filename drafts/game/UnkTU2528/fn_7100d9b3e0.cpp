// Unit 2528: the file path id for a name: the name lower-cased, with backslashes made slashes and then
// slashes made '#', its extension (from the last '.') and anything up to the last ':' dropped.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

#include "../../../src/game_aca57c/UnkBufferString.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

using UnkPathName_ld9b3e0 = UnkBufferString<0x100, g_unk_7101a194b8>;

// Copies `src` lower-cased (ASCII) into `dst`.
static inline void ToLower(char* dst, const char* src) {
    dst[0] = '\0';
    u32 length = __builtin_strlen(src);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            dst[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        dst[length] = '\0';
    }
}

// Copies `src` into `dst` with every `from` made `to`.
static inline void ReplaceChar(char* dst, const CRntString& src, char from, char to) {
    u32 length = src.length;
    dst[length] = '\0';
    for (u32 i = 0; i < length; ++i) {
        char c = src.data[i];
        dst[i] = c == from ? to : c;
    }
}

static inline UnkPathName_ld9b3e0 Lowered(const char* text) {
    char buffer[0x200];
    ToLower(buffer, text);
    return UnkPathName_ld9b3e0(buffer);
}

static inline UnkPathName_ld9b3e0 Replaced(const UnkPathName_ld9b3e0& name, char from, char to) {
    char buffer[0x200];
    ReplaceChar(buffer, *name.Str(), from, to);
    return UnkPathName_ld9b3e0(buffer);
}

// Keeps `length` characters of the name from `from` on.
static inline void Keep(UnkPathName_ld9b3e0& name, const char* from, u32 length) {
    UnkPathName_ld9b3e0 part;
    CRntString_AssignPrefix(part.Str(), from, length);
    CRntString_Assign(name.Str(), part.Str());
}

CFilePathStrId fn_7100d9b3e0(const CStrId& id) {
    ++id.data->reads;
    UnkPathName_ld9b3e0 path = Replaced(Replaced(Lowered(id.data->text), '\\', '/'), '/', '#');
    s32 dot = CRntString_FindLastChar(path.Str(), '.', -1);
    if (dot > 0)
        Keep(path, path.Str()->data, dot);
    s32 colon = CRntString_FindLastChar(path.Str(), ':', -1);
    if (colon > 0) {
        u32 start = colon + 1;
        u32 length = path.Str()->length - start;
        Keep(path, path.Str()->data + start, length);
    }
    const char* text = path.Str()->data;
    return CFilePathStrId(text ? text : g_unk_710162e2ed, true);
}
