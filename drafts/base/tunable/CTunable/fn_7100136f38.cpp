// base::tunable::CTunable's unit: sets the string id at +0x18 to the id of "<first>|<second>", the text of
// the two ids at +0x8 and +0x10 joined with a bar; when the first is the empty id, to the second.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The pooled data as seen here: the string at +0x10 and a counter at +0x44 bumped whenever the
// string is read.
struct UnkStringText136f38 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 field_0x44;
    };
};

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
    void Build(const CRntString& str, bool flag) { fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, flag); }
};

static inline const CRntString& TextOf(const CStrId& id) {
    ++reinterpret_cast<UnkStringText136f38*>(id.data)->field_0x44;
    return reinterpret_cast<UnkStringText136f38*>(id.data)->str;
}

void fn_7100136f38(UnkIdPair136f38* self) {
    if (self->first.data != &g_unk_7101d47108->empty) {
        UnkNewId136f38 id;
        CRntString head = fn_710007ef90(&TextOf(self->first), "|");
        CRntString full = fn_710007eee0(&head, &TextOf(self->second));
        id.Build(full, true);
        self->joined = static_cast<CStrId&&>(*reinterpret_cast<CStrId*>(&id));
    } else {
        self->joined = self->second;
    }
}
