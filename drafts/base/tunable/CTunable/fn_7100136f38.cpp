// base::tunable::CTunable's unit: sets the string id at +0x18 to the id of "<first>|<second>", the text of
// the two ids at +0x8 and +0x10 joined with a bar; when the first is the empty id, to the second.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkIdPair136f38 {
    u8 pad_0x0[0x8];
    CStrId first;
    CStrId second;
    CStrId joined;
};

// A string id built from a string (fn_7100000850) and handed over by the move assignment, so it needs
// no destructor of its own.
struct UnkNewId136f38 {
    base::global::UnkStringData* data;
    UnkNewId136f38(const CRntString& str, bool flag) { CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, flag); }
    CStrId&& Move() { return static_cast<CStrId&&>(*reinterpret_cast<CStrId*>(this)); }
};

// The id's pooled string, counting the read. The pooled text is a whole CRntString in the original
// (UnkStringData lays out only its text and key).
static inline const CRntString& TextOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The string concatenations, out of line.
static inline CRntString operator+(const CRntString& a, const char* b) { return CRntString_ConcatCString(&a, b); }
static inline CRntString operator+(const CRntString& a, const CRntString& b) { return CRntString_Concat(&a, &b); }

void fn_7100136f38(UnkIdPair136f38* self) {
    if (self->first.data != &g_unk_7101d47108->empty) {
        self->joined = UnkNewId136f38(TextOf(self->first) + "|" + TextOf(self->second), true).Move();
    } else {
        self->joined = self->second;
    }
}
