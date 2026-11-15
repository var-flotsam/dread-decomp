// Unit 2200: the move helper of the iterator value over a reflected array of 0x40-byte elements (ids and
// words, moved out of the source). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkTU2200Elem_lc91f48 {
    base::UnkStrIdMember name;                         // 0x0
    u32 field_0x8;
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;     // 0x10
    base::UnkStrIdMember ids[3];                       // 0x18
    u64 field_0x30;
    u32 field_0x38;

    UnkTU2200Elem_lc91f48(UnkTU2200Elem_lc91f48&& o)
        : name(static_cast<base::UnkStrIdMember&&>(o.name)),
          field_0x8(o.field_0x8),
          path(static_cast<base::UnkStrIdMemberOf<g_unk_7101d47200>&&>(o.path)),
          ids{static_cast<base::UnkStrIdMember&&>(o.ids[0]), static_cast<base::UnkStrIdMember&&>(o.ids[1]),
              static_cast<base::UnkStrIdMember&&>(o.ids[2])},
          field_0x38(o.field_0x38),
          field_0x30(o.field_0x30) {}
    ~UnkTU2200Elem_lc91f48() asm("fn_7100c8a490");  // The element's destructor.
};

using UnkTU2200Iter_lc91878 = base::UnkIterValue<UnkTU2200Elem_lc91f48>;

void fn_7100c91878(void* dst, const void* src) {
    UnkTU2200Iter_lc91878* self = static_cast<UnkTU2200Iter_lc91878*>(dst);
    UnkTU2200Iter_lc91878* other = static_cast<UnkTU2200Iter_lc91878*>(const_cast<void*>(src));
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) UnkTU2200Iter_lc91878::Value(static_cast<UnkTU2200Iter_lc91878::Value&&>(other->value));
    } else if (other->state == 2) {
        self->state = 1;
        UnkTU2200Iter_lc91878::Ref r = other->ref;
        new (&self->value) UnkTU2200Iter_lc91878::Value{static_cast<u32&&>(*const_cast<u32*>(r.index)),
                                                         static_cast<UnkTU2200Elem_lc91f48&&>(*r.element)};
    }
}
