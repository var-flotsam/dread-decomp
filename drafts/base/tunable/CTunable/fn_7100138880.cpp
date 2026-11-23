// base::tunable::CTunable's unit: removes the object whose key equals `key` from a registry of named
// objects: its key is destroyed, then the last entry is moved into its place.
// Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkTunableObject138880;

struct UnkKeyedObjects138880 {
    UnkTunableObject138880** objects;
    base::UnkNamedEntry* keys;
    // A union member: the original reloads the count after storing the object pointer.
    union {
        u32 count;
    };

    UnkTunableObject138880** Find(const base::UnkNamedEntry* key) const {
        for (u32 i = 0; i < count; ++i)
            if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&keys[i]),
                              reinterpret_cast<const UnkCompared_0x30*>(key)))
                return &objects[i];
        return objects + count;
    }
};

void fn_7100138880(UnkKeyedObjects138880* self, const base::UnkNamedEntry* key) {
    UnkTunableObject138880** slot = self->Find(key);
    if (slot == self->objects + self->count)
        return;
    u32 index = slot - self->objects;
    CRntString_Destroy(reinterpret_cast<CRntString*>(&self->keys[index]));
    u32 last = self->count - 1;
    if (index != last) {
        self->objects[index] = self->objects[last];
        CRntString_MoveConstruct(reinterpret_cast<CRntString*>(&self->keys[index]),
                      reinterpret_cast<CRntString*>(&self->keys[self->count - 1]));
        CRntString_Destroy(reinterpret_cast<CRntString*>(&self->keys[self->count - 1]));
        last = self->count - 1;
    }
    self->count = last;
}
