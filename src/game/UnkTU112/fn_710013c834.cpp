// Unit 112: a copy of a string with every occurrence of one text replaced by another.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkTU112.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// The two strings' texts one after the other.
static inline CRntString operator+(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }

// Assigns another string's text.
static inline void Assign(CRntString* self, const CRntString& other) { fn_710007eaa0(self, &other); }

UnkTunableLine fn_710013c834(const CRntString* str, const char* from, const char* to) {
    if (str->length == 0)
        return UnkTunableLine(*str);

    UnkTunableLine result(*str);
    u32 fromLength = from ? strlen(from) : 0;
    UnkTunableLine replacement(to);
    s32 at = fn_710007fb54(result.Str(), from, 0);
    while (at != -1) {
        UnkTunableLine head(result.Str()->data, at);
        u32 end = at + fromLength;
        u32 restLength = result.Str()->length - end;
        UnkTunableLine tail(result.Str()->data + end, restLength);
        Assign(result.Str(), *head.Str() + *replacement.Str() + *tail.Str());
        at = fn_710007fb54(result.Str(), from, head.Str()->length + replacement.Str()->length);
    }
    return UnkTunableLine(*result.Str());
}
