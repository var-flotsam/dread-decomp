// Whether an id is known to a keyed table: queued for addition, or already in the table.
// It first initialises two function-local static masks it doesn't use.
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkMaskA {
    u32 value;
    UnkMaskA() : value(0) {}
};

struct UnkMaskB {
    u32 value;
    UnkMaskB(u8 v) : value(v) {}
};

inline const UnkMaskA& UnkMaskNone_7101d57cc8() {
    static UnkMaskA s_none;
    return s_none;
}

inline const UnkMaskB& UnkMaskAll_7101d57cb8() {
    static UnkMaskB s_all((u8)~*reinterpret_cast<const u8*>(&UnkMaskNone_7101d57cc8().value));
    return s_all;
}

// The queued addition with the key, or the end of the queue.
static inline UnkPendingAdd* FindPending(base::UnkVector<UnkPendingAdd>& list, u64 key) {
    for (u32 i = 0; i < list.count; ++i)
        if (list.data[i].key == key)
            return &list.data[i];
    return &list.data[list.count];
}
static inline UnkPendingAdd* EndPending(base::UnkVector<UnkPendingAdd>& list) { return &list.data[list.count]; }

bool fn_7100a2be94(UnkTimelineComponent* self, const u64* id) {
    u64 key = *id;
    UnkMaskAll_7101d57cb8();
    UnkPendingAdd* p = FindPending(self->pending_add, key);
    if (p == EndPending(self->pending_add)) {
        UnkTableEntry* it = self->table.Find(*id);
        if (it == self->table.End())
            return false;
    }
    return true;
}
