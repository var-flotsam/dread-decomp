// CAIComponent unit (tools/tu_map.py unit 2702): builds a table of ten (string id, value) pairs, all empty,
// then copies up to ten given pairs into it in order (a list of more than ten is ignored). Names and layouts
// are placeholders.
#include <new>

#include "../../../src/game_s7/UnkStaticInitS7.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdInline;
using unk_static_init::UnkIdValue;

struct UnkIdValueEntry_l0f41a0 {
    CStrId id;
    u32 value;

    explicit UnkIdValueEntry_l0f41a0(const UnkIdValue& item)
        : id(reinterpret_cast<const CStrId&>(item.id)), value(item.value) {}
};

struct UnkIdValueTable_l0f41a0 {
    UnkIdValueEntry_l0f41a0 entries[10];
};

void fn_71010f41a0(void* self, const UnkIdValue* items, u32 count) {
    UnkIdValueTable_l0f41a0* table = static_cast<UnkIdValueTable_l0f41a0*>(self);
    for (UnkIdValueEntry_l0f41a0& entry : table->entries)
        unk_static_init::MakeEmptyId(&entry.id);
    if (count > 10)
        return;
    u32 i = 0;
    for (const UnkIdValue* it = items; it != items + count; ++it) {
        UnkIdValueEntry_l0f41a0 entry(*it);
        table->entries[i++] = entry;
    }
}
