// A table of counted resources (at 0) and string ids (at 8) sharing a count, cleared: the ids are
// released, then the resources (fn_710001737c frees one whose count reaches zero), and the count
// is zeroed; the arrays stay. The same code in the CMinimapManager::CTunableMinimapManager unit
// (fn_7100eb346c) and the CCharClass unit (fn_71011ac8e8). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkResKey.h"
#include "types.h"
using base::UnkStrIdMember;
struct UnkResIdTable {  // the head of a larger object
    UnkCountedResource** refs;
    UnkStrIdMember* ids;
    u32 count;
};
__attribute__((always_inline)) static inline void Clear(UnkResIdTable* self) {
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
void fn_7100eb346c(UnkResIdTable* self) { Clear(self); }
void fn_71011ac8e8(UnkResIdTable* self) { Clear(self); }
