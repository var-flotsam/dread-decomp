// Unit 399 (tools/tu_map.py): finds the entry of a registry's pointer array (+0xa0, count +0xa8) whose
// name (the first word of what fn_71002dba48 returns for it) is `key`; null when none is. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU399.h"
#include "game/animtree/CAnimTreeElement.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntryRegistry_l2d57ac {
    u8 pad_0x0[0xa0];
    UnkEntry** data;  // 0xa0
    u32 count;        // 0xa8
};

// The name fn_71002dba48 hands back for an entry.
static inline const CStrId& NameOf(UnkEntry* entry) {
    return *reinterpret_cast<const CStrId*>(fn_71002dba48(reinterpret_cast<UnkPairOwner*>(entry)));
}

UnkEntry* fn_71002d57ac(void* registry, const void* key) {
    UnkEntryRegistry_l2d57ac* self = static_cast<UnkEntryRegistry_l2d57ac*>(registry);
    UnkEntry** found = self->data;
    UnkEntry** end = found + self->count;
    {
        CStrId name(*static_cast<const CStrId*>(key));
        for (; found != end; ++found) {
            if (NameOf(*found).data == name.data)
                break;
        }
    }
    if (found == self->data + self->count)
        return nullptr;
    return *found;
}
