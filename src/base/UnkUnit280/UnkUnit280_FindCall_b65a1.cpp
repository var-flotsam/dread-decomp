// Unit 280 of tools/tu_map.py (no reflected class): finds the entry whose id (its first word) is
// `id` in the receiver's array of entry pointers and, when there is one and `target` is set, returns
// what fn_7100202cdc does with it and the remaining arguments; false otherwise. Names are placeholders.
#include "types.h"

struct UnkUnit280Entry_b65a1 {
    u32 id;
};

struct UnkUnit280List_b65a1 {
    UnkUnit280Entry_b65a1** entries;
    u32 count;
};

bool fn_7100202cdc(UnkUnit280Entry_b65a1* entry, void* target, void* a, bool b, u32 c, void* d);

bool fn_71001fcefc(UnkUnit280List_b65a1* self, u32 id, void* target, void* a, bool b, u32 c, void* d) {
    UnkUnit280Entry_b65a1* found = nullptr;
    for (UnkUnit280Entry_b65a1** it = self->entries, **end = self->entries + self->count; it != end; ++it) {
        if ((*it)->id == id) {
            found = *it;
            break;
        }
    }
    if (!found || !target)
        return false;
    return fn_7100202cdc(found, target, a, b, c, d);
}
