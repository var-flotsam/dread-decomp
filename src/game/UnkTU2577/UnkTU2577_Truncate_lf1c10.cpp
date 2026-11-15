// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ef1c10 sets the count of an
// array of 0x18-byte entries (two string ids and a word; base::global::CRntVector's layout), first destroying the
// entries from the new count on. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkIdEntry_lf1c10 {
    base::UnkStrIdMember field_0x0;
    base::UnkStrIdMember field_0x8;
    void* field_0x10;
};
static_assert(sizeof(UnkIdEntry_lf1c10) == 0x18, "layout");

void fn_7100ef1c10(void* vector, u32 count) {
    base::global::CRntVector<UnkIdEntry_lf1c10>* self = static_cast<base::global::CRntVector<UnkIdEntry_lf1c10>*>(vector);
    for (u32 i = count; i < self->count; ++i)
        self->data[i].~UnkIdEntry_lf1c10();
    self->count = count;
}
