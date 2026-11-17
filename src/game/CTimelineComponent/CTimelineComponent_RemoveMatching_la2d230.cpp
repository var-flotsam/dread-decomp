// CTimelineComponent's unit (1542): for every table entry whose object's name contains the given name,
// collects that name id, then hands each collected id's key to fn_7100a2b7b8. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1134.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// Up to eight string ids on the stack.
struct UnkStrIdList_la2d230 {
    u32 count;
    u8 storage[8 * sizeof(CStrId)];

    CStrId* items() { return reinterpret_cast<CStrId*>(storage); }

    UnkStrIdList_la2d230() : count(0) {}
    ~UnkStrIdList_la2d230() {
        for (u64 i = 0; i < count; i++)
            items()[i].~CStrId();
    }

    void PushBack(const CStrId& id) {
        new (&items()[count]) CStrId(id);
        ++count;
    }
};

// The name id of a table entry's object.
static inline CStrId& EntryName(UnkTableEntry* entry) {
    return *reinterpret_cast<CStrId*>(fn_710070d7f8(reinterpret_cast<UnkObj_710117af10*>(entry->field_0x0)));
}

void fn_7100a2d230(UnkTimelineComponent* self, const CStrId& name) {
    if (name.data == &g_unk_7101d47108->empty)
        return;
    UnkStrIdList_la2d230 found;
    for (UnkTableEntry *entry = self->table.values, *end = entry + self->table.count; entry != end; ++entry) {
        CStrId& id = EntryName(entry);
        ++id.data->reads;
        if (id.data == &g_unk_7101d47108->empty)
            continue;
        const CRntString* text = &id.data->String();
        if (fn_710007fb9c(text, name.Text()))
            found.PushBack(EntryName(entry));
    }
    for (CStrId *id = found.items(), *end = id + found.count; id != end; ++id) {
        u64 key = id->data->Key();
        fn_7100a2b7b8(self, &key);
    }
}
