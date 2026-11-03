// A function in the CCollisionComponent unit: finds the entry of the pointer array at +0x70 (count at
// +0x78) whose key at +0x18 is the one at +0x30 of the argument's object. Names and layouts are
// placeholders.
#include "types.h"

struct UnkCollisionEntry {
    u8 pad_0x0[0x18];
    void* key;
};
struct UnkCollisionQuery {
    u8 pad_0x0[0x30];
    void* key;
};
struct UnkCollisionEntryList {
    u8 pad_0x0[0x70];
    UnkCollisionEntry** entries;
    u32 count;
};

// Declared as src/game/CPlayerMovement/CPlayerMovement_CachedComponent.cpp declares it.
struct UnkPlayerMovementSource;

void* fn_710086406c(UnkPlayerMovementSource* source, const void* type) {
    UnkCollisionEntryList* self = reinterpret_cast<UnkCollisionEntryList*>(source);
    UnkCollisionQuery* const* query = static_cast<UnkCollisionQuery* const*>(type);
    for (UnkCollisionEntry **it = self->entries, **end = it + self->count; it != end; ++it) {
        if ((*query)->key == (*it)->key)
            return *it;
    }
    return nullptr;
}
