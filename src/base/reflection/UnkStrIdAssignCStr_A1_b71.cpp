// tools/tu_map.py unit 83 (base::reflection): a string id assigned from an object that starts with
// its text (built through fn_71000003d4 and moved in; nothing happens on self-assignment). Names
// are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkCStrHolder_A1b71 {
    const char* str;
};

CStrId* fn_71000cd2a4(CStrId* self, const UnkCStrHolder_A1b71* other) {
    if (other != static_cast<const void*>(self)) {
        CStrId id(other->str, true);
        self->ReleaseAndTake(id);
    }
    return self;
}
