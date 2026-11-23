// Unit 325: the kind of a movie file by its name's extension: 0 for .mp4 and .m4a, 1 for .mkv,
// 2 for .webm, 0x7fffffff for anything else. Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// Whether the string's text equals a C string (src/base/global/CRntString.cpp).
bool CRntString_EqualsIgnoreCase(const CRntString* self, const char* str);

// A string on 0x100 bytes of inline storage.
struct UnkNameString_220880 : UnkBufferString<0x100, g_unk_7101a194b8> {
    explicit UnkNameString_220880(const char* str) : UnkBufferString(str) {}
    // The first `length` characters of `str`.
    UnkNameString_220880(const char* str, u32 length) { CRntString_AssignPrefix(Str(), str, length); }
    explicit UnkNameString_220880(const CRntString& str) { CRntString_Assign(Str(), &str); }

    CRntString&& Take() { return static_cast<CRntString&&>(*Str()); }
};

u32 fn_7100220880(const char* name) {
    UnkNameString_220880 path(name);
    s32 dot = CRntString_FindLastChar(path.Str(), '.', -1);
    // The extension: the last `count` characters, from the last dot on (empty without one).
    u32 count = path.Str()->length - dot;
    UnkNameString_220880 ext(dot < 0 ? CRntString(g_unk_7101d58d10)
                                     : CRntString(UnkNameString_220880(path.Str()->data + (path.Str()->length - count),
                                                                       count)
                                                      .Take()));
    u32 kind;
    if (CRntString_EqualsIgnoreCase(ext.Str(), ".mp4") || CRntString_EqualsIgnoreCase(ext.Str(), ".m4a"))
        kind = 0;
    else if (CRntString_EqualsIgnoreCase(ext.Str(), ".mkv"))
        kind = 1;
    else if (CRntString_EqualsIgnoreCase(ext.Str(), ".webm"))
        kind = 2;
    else
        kind = 0x7fffffff;
    return kind;
}
