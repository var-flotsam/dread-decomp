#pragma once

// The object at +0x138 of the owners in src/game_lo/ (fn_71001436f4, fn_71003ad278): a table of
// slots, each with an id, at +0x78, and a list of the nine entries (an id and a kind each) that
// fn_71001c2764 reorders after a request on the table succeeds. The request is blocked while the
// flag of game/UnkGlobal_710b372a98.h is set. The object is defined in src/game_lo/; only the
// members the callers share are spelled out, and the names and the layout are placeholders.

#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "types.h"

// The slot table: the count of its slots is at +0x2c.
struct UnkSlotTable {
    u8 unk_0x0[0x2c];
    u32 count;
};

// Sets the value of the slot `id` and `kind` of the table in set `mode` (0 or 1): stores it in the
// table and in the slot's record, and in the record's target when there is one. False for another
// mode.
bool fn_71001996e0(UnkSlotTable* table, u32 id, u32 kind, base::math::UnkVector4D value, u32 mode);

// The value of the slot `id` and `kind` of the table in set `mode` (0 or 1) into `out`. The table
// is an untyped pointer here: converting UnkSlots::info to it orders the callers' loads as the
// original does (src/game/UnkSlotBlend.cpp).
bool fn_71001997c8(void* table, u32 id, u32 kind, base::math::CVector4D* out, u32 mode);

struct UnkSlotEntry {
    u8 unk_0x0[0x28];
    u32 field_0x28;  // the slot id
    u32 field_0x2c;  // the kind
};

struct UnkSlots {
    u8 unk_0x0[0x78];
    UnkSlotTable* info;
    u8 unk_0x80[0x308 - 0x80];
    UnkSlotEntry* entries[9];
    // A union member: the original reloads the count after each pointer store.
    union {
        u32 count;
    };
};
static_assert(__builtin_offsetof(UnkSlots, info) == 0x78, "UnkSlots placeholder layout");
static_assert(__builtin_offsetof(UnkSlots, entries) == 0x308, "UnkSlots placeholder layout");
static_assert(__builtin_offsetof(UnkSlots, count) == 0x350, "UnkSlots placeholder layout");

// Forwards a request for the slot `id` and `kind`, with a vector, to the slot table, unless the
// global flag blocks it. When it succeeds and `remove` is set, moves every entry with that id
// and kind past the end of the list (swapping it with the last one and shrinking the count).
// Returns whether the request was made.
bool fn_71001c2764(UnkSlots* self, u32 id, u32 kind, const base::math::CVector4D* value, u32 mode, bool remove);

// Like fn_71001c2764, but looks the id up first: resolves the slot registered under `key` and
// requests it with `color` (unless the global flag blocks it); on success moves the entries with
// that id and kind past the end of the list. Defined in src/game_lo/fn_71001c28f8.cpp.
bool fn_71001c28f8(UnkSlots* self, u64 key, u32 kind, const base::color::CColor4F* color, u32 mode);
