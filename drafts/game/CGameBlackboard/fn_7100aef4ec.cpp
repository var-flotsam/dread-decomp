// CGameBlackboard unit: removes an entry from two parallel arrays (owned objects and their ids) by moving the
// last entry into its place. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CGameBlackboard.h"
#include "types.h"

void operator delete(void*);

struct UnkOwnedEntry;

struct UnkEntryArrays {
    UnkOwnedEntry** entries;
    base::global::CStrId* ids;
    u32 count;

    static void Destroy(UnkOwnedEntry* entry) {
        if (entry) {
            fn_7100aef674(entry);
            ::operator delete(entry);
        }
    }
};

UnkOwnedEntry** fn_7100aef4ec(UnkEntryArrays* self, UnkOwnedEntry** pos) {
    u32 index = (u32)(pos - self->entries);
    UnkOwnedEntry* removed = self->entries[index];
    self->entries[index] = nullptr;
    UnkEntryArrays::Destroy(removed);
    self->ids[index].Reset();

    u32 last = self->count - 1;
    if (last != index) {
        UnkOwnedEntry* moved = self->entries[last];
        self->entries[last] = nullptr;
        self->entries[index] = moved;

        UnkOwnedEntry* tail = self->entries[self->count - 1];
        self->entries[self->count - 1] = nullptr;
        UnkEntryArrays::Destroy(tail);

        base::global::CStrId* ids = self->ids;
        u32 end = self->count - 1;
        ids[index].data = ids[end].data;
        ids[end].data = &g_unk_7101d47108->empty;
        if (g_unk_7101d47108) {
            self->ids[self->count - 1].Reset();
        }
    }
    self->count = self->count - 1;
    return &self->entries[index];
}
