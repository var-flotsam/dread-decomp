// Unit 2566 (CMinimapManager::CTunableMinimapManager): a copy of a string with every occurrence of
// one text replaced by another. Names and layouts are placeholders.
#include <string.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// A string on 0x40 bytes of inline storage.
struct UnkString40_a5178 : UnkBufferString<0x40, g_unk_7101a1b678> {
    explicit UnkString40_a5178(const char* str) : UnkBufferString(str) {}
    // The first `length` characters of `str`.
    UnkString40_a5178(const char* str, u32 length) { fn_710007e7f8(Str(), str, length); }
    explicit UnkString40_a5178(const CRntString& str) { fn_710007eaa0(Str(), &str); }
};

// The two strings' texts one after the other.
static inline CRntString operator+(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }

// Assigns another string's text.
static inline void Assign(CRntString* self, const CRntString& other) { fn_710007eaa0(self, &other); }

UnkString40_a5178 fn_7100ea5178(const CRntString* text, const char* from, const char* to) {
    if (text->length == 0)
        return UnkString40_a5178(*text);

    UnkString40_a5178 result(*text);
    u32 fromLength = from ? strlen(from) : 0;
    UnkString40_a5178 replacement(to);
    s32 at = fn_710007fb54(result.Str(), from, 0);
    while (at != -1) {
        UnkString40_a5178 head(result.Str()->data, at);
        u32 end = at + fromLength;
        u32 restLength = result.Str()->length - end;
        UnkString40_a5178 tail(result.Str()->data + end, restLength);
        Assign(result.Str(), *head.Str() + *replacement.Str() + *tail.Str());
        at = fn_710007fb54(result.Str(), from, head.Str()->length + replacement.Str()->length);
    }
    return UnkString40_a5178(*result.Str());
}
