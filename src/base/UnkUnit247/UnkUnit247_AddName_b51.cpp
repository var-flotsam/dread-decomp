// Unit 247: adds a name to the binder's names (+0xd8) unless it is there already. Names and
// layouts are placeholders inferred from the code.

#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkBinderNames_71001bbfa8 {
    u8 pad_0x0[0xd8];
    base::UnkStrIdVec names;
};

void fn_71001bbfa8(UnkBinderNames_71001bbfa8* self, const base::global::CStrId* name) {
    base::UnkStrIdVec& names = self->names;
    u64 n = names.count;
    {
        const base::global::CStrId* p = names.data;
        for (; n; --n, ++p)
            if (p->data == name->data)
                return;
    }
    names.PushBack(*name);
}
