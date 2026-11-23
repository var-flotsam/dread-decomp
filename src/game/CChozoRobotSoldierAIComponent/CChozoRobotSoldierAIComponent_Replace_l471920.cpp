// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): a copy of a string with every
// occurrence of one text replaced by another. Names and layouts are placeholders.
#include <string.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// A string on 0x80 bytes of inline storage.
struct UnkString80_l471920 : UnkBufferString<0x80, g_unk_7101a1b750> {
    explicit UnkString80_l471920(const char* str) : UnkBufferString(str) {}
    // The first `length` characters of `str`.
    UnkString80_l471920(const char* str, u32 length) { CRntString_AssignPrefix(Str(), str, length); }
    explicit UnkString80_l471920(const CRntString& str) { CRntString_Assign(Str(), &str); }
};

// The two strings' texts one after the other.
static inline CRntString operator+(const CRntString& a, const CRntString& b) { return CRntString_Concat(&a, &b); }

// Assigns another string's text.
static inline void Assign(CRntString* self, const CRntString& other) { CRntString_Assign(self, &other); }

UnkString80_l471920 fn_7100471920(const CRntString* text, const char* from, const char* to) {
    if (text->length == 0)
        return UnkString80_l471920(*text);

    UnkString80_l471920 result(*text);
    u32 fromLength = from ? strlen(from) : 0;
    UnkString80_l471920 replacement(to);
    s32 at = CRntString_Find(result.Str(), from, 0);
    while (at != -1) {
        UnkString80_l471920 head(result.Str()->data, at);
        u32 end = at + fromLength;
        u32 restLength = result.Str()->length - end;
        UnkString80_l471920 tail(result.Str()->data + end, restLength);
        Assign(result.Str(), *head.Str() + *replacement.Str() + *tail.Str());
        at = CRntString_Find(result.Str(), from, head.Str()->length + replacement.Str()->length);
    }
    return UnkString80_l471920(*result.Str());
}
