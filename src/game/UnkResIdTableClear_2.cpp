// Copy of fn_71011ac8e8 (src/game/UnkResIdTableClear.cpp): a table of counted resources (at 0) and
// string ids (at 8) sharing a count, cleared; the arrays stay. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkResKey.h"
#include "types.h"
using base::UnkStrIdMember;
struct UnkResIdTable2 {  // the head of a larger object
    UnkCountedResource** refs;
    UnkStrIdMember* ids;
    u32 count;
};
void fn_71002f7930(UnkResIdTable2* self) {
    if (self->ids && self->count)
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdMember();
    if (self->refs && self->count)
        for (u64 i = 0; i < self->count; ++i)
            if (UnkCountedResource* p = self->refs[i])
                if (--p->ref_count == 0)
                    fn_710001737c(p);
    self->count = 0;
}
