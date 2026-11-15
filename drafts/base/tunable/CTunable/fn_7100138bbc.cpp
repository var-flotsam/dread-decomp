// base::tunable::CTunable's unit: equality of two registries of named objects: the same count, and every
// key of the first is in the second (compared by fn_710007f120) with the same object pointer.
// Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "game/UnkTU57.h"
#include "types.h"

struct UnkTunableObject138bbc;

struct UnkKeyedObjects138bbc {
    UnkTunableObject138bbc** objects;
    base::UnkNamedEntry* keys;
    u32 count;

    UnkTunableObject138bbc** Find(const base::UnkNamedEntry* key) const {
        for (u32 i = 0; i < count; ++i)
            if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&keys[i]),
                           reinterpret_cast<const UnkCompared_0x30*>(key)))
                return &objects[i];
        return objects + count;
    }
};

bool fn_7100138bbc(const UnkKeyedObjects138bbc* a, const UnkKeyedObjects138bbc* b) {
    if (a->count != b->count)
        return false;
    for (u32 i = 0; i < a->count; i++) {
        const base::UnkNamedEntry& key = a->keys[i];
        UnkTunableObject138bbc** slot = b->Find(&key);
        if (slot == b->objects + b->count)
            return false;
        if (*slot != a->objects[i])
            return false;
    }
    return true;
}
