// In CFXComponent's unit (unit 2712 of tools/tu_map.py): the effect an owner keeps under a name's hash,
// from its parallel arrays at +0x50 (0x20-byte entries whose first word is the effect) and +0x58 (the
// names, whose hash is at +0x30 of the string), or null. Names and layouts are placeholders.
#include "types.h"

struct UnkComponent;

struct UnkFXName {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkFXEntry {
    UnkComponent* effect;
    u8 pad_0x8[0x18];
};

struct UnkObject {
    u8 pad_0x0[0x50];
    UnkFXEntry* values;
    UnkFXName** names;
    u32 count;
};

// NON_MATCHING(minor): the original compares the slot with the end before testing it for null (in its
// conditional compare); same behaviour.
UnkComponent* fn_7101152b90(UnkObject* self, u64 hash) {
    UnkFXEntry* slot = [&] {
        for (u32 i = 0; i < self->count; ++i)
            if (self->names[i]->hash == hash)
                return &self->values[i];
        return &self->values[self->count];
    }();
    if (slot == &self->values[self->count])
        return nullptr;
    return slot ? slot->effect : nullptr;
}
