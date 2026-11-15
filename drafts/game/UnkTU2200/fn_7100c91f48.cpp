// Unit 2200: erases the element at an index from an array of 0x40-byte elements (destroys it, moves the
// later ones down). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
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
          field_0x30(o.field_0x30),
          field_0x38(o.field_0x38) {}
    ~UnkTU2200Elem_lc91f48() asm("fn_7100c8a490");  // The element's destructor.
};

void fn_7100c91f48(base::global::CRntVector<UnkTU2200Elem_lc91f48>* self, const u32* index) {
    UnkTU2200Elem_lc91f48* base = self->data;
    u64 offset = (u64)*index * sizeof(UnkTU2200Elem_lc91f48);
    UnkTU2200Elem_lc91f48* p = reinterpret_cast<UnkTU2200Elem_lc91f48*>(reinterpret_cast<char*>(base) + offset);
    u32 first = (u32)(p - base);
    if (first < self->count) {
        u64 i = first;
        self->data[i].~UnkTU2200Elem_lc91f48();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkTU2200Elem_lc91f48* next = &self->data[i + 1];
            new (next - 1) UnkTU2200Elem_lc91f48(static_cast<UnkTU2200Elem_lc91f48&&>(*next));
            self->data[i + 1].~UnkTU2200Elem_lc91f48();
        }
        self->count = self->count - 1;
    }
}
