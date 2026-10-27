#pragma once

// A 4-byte key of tools/tu_map.py unit 361 (0x71002874bc-0x7100287598, no reflected class): an id
// (u16) and a kind (u8). fn_71002874d0 builds one and the empty fn_71002874dc (base/UnkEmptyDtorElem.h)
// destroys it; fn_71002874e0 hashes the pair. Held in a list read from a file
// (src/base/UnkKeyListRead.cpp) and in the nodes of a chained hash table (src/game_s2/fn_71002b26fc.cpp).
// Names and layout are placeholders.

#include "base/UnkEmptyDtorElem.h"
#include "types.h"

struct UnkIdKindKey;

// Stores `id` and `kind` in the key (its constructor).
void fn_71002874d0(UnkIdKindKey* self, u16 id, u8 kind);

struct UnkIdKindKey {
    u16 id;
    u8 kind;
    u8 pad_0x3;

    UnkIdKindKey(u16 id_, u8 kind_) { fn_71002874d0(this, id_, kind_); }
    ~UnkIdKindKey() { fn_71002874dc(this); }
};
static_assert(sizeof(UnkIdKindKey) == 0x4, "UnkIdKindKey layout");
