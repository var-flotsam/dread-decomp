#pragma once

// The component in the slot at +0x2e8 of game/UnkActor.h, as the hand-written callers see it: a
// table of entries keyed by id (parallel arrays), the additions and removals queued while the
// table is locked, and a timed list of delayed additions. tools/tu_map.py puts its functions
// (fn_7100a2b7b8, fn_7100a2be94, fn_7100a2cb7c ...) in the unit it names after
// CTimelineComponent, so the placeholder is named after that component; whether the slot is that
// component is unverified. The functions are defined in src/game_hi/ (one file each); fn_7100d7c828
// reaches the component through the actor. Only the members the sources share are spelled out; the
// names and the layout are placeholders.

#include "base/UnkVector.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkTableEntry {
    u64 field_0x0;
    u8 unk_0x8[0x38];
};

// Values and keys in parallel arrays.
struct UnkKeyedTable {
    UnkTableEntry* values;
    u64* keys;
    u32 count;

    UnkTableEntry* Find(const u64& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
    UnkTableEntry* End() { return &values[count]; }
    u64 IndexOf(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return i;
        return count;
    }
};

struct UnkPendingAdd {
    u64 key;
    u32 value;

    UnkPendingAdd(u64 key, u32 value) : key(key), value(value) {}
    // The key is copied bitwise (the original reloads the array's count and data pointer
    // after the copies).
    UnkPendingAdd(const UnkPendingAdd& other) {
        __builtin_memcpy(&key, &other.key, sizeof(key));
        value = other.value;
    }
};

// A delayed addition: the next one in the list, and the time left.
struct UnkTimedEntry {
    UnkPendingAdd item;
    f32 delay;
    UnkTimedEntry* next;
};

struct UnkTimelineComponent {
    u8 unk_0x0[0x68];
    UnkKeyedTable table;
    u8 unk_0x80[0x10];
    bool locked;                                 // while set, changes to the table are queued
    base::UnkVector<UnkPendingAdd> pending_add;  // the additions queued
    base::UnkVector<u64> pending_remove;         // the ids to remove, queued
    u8 unk_0xe8[0x98];
    // A union member: no type-based alias info, so the count is reloaded after the link
    // stores.
    union {
        u32 timed_count;
    };
    UnkTimedEntry* timed_head;
    UnkTimedEntry* timed_tail;
    base::UnkAllocator* timed_allocator;
    u32 timed_flags;
};
static_assert(__builtin_offsetof(UnkTimelineComponent, table) == 0x68, "UnkTimelineComponent placeholder layout");
static_assert(__builtin_offsetof(UnkTimelineComponent, locked) == 0x90, "UnkTimelineComponent placeholder layout");
static_assert(__builtin_offsetof(UnkTimelineComponent, pending_add) == 0x98, "UnkTimelineComponent placeholder layout");
static_assert(__builtin_offsetof(UnkTimelineComponent, pending_remove) == 0xc0, "UnkTimelineComponent placeholder layout");
static_assert(__builtin_offsetof(UnkTimelineComponent, timed_count) == 0x180, "UnkTimelineComponent placeholder layout");
static_assert(__builtin_offsetof(UnkTimelineComponent, timed_flags) == 0x1a0, "UnkTimelineComponent placeholder layout");

// Removes the entry with the given id (-1 = none): right away unless the table is locked, in
// which case the id is queued.
void fn_7100a2b7b8(UnkTimelineComponent* self, const u64* id);
// Whether an id is known: queued for addition, or already in the table.
bool fn_7100a2be94(UnkTimelineComponent* self, const u64* id);
// The entry for an id in a resource reached through the owner (+0x30, its +0x28 when that is of
// the expected class; the id may be remapped first), not in the table above; null when there is
// none. The callers only test it or pass it on (fn_7100a2c200).
struct UnkTimelineResourceEntry;
UnkTimelineResourceEntry* fn_7100a2c13c(UnkTimelineComponent* self, const u64* id);
// Whether the id is in the table (fn_7100d7c828 skips adding the default then).
bool fn_7100a2cfe8(UnkTimelineComponent* self, const u64* id);
// fn_7100a2cb7c (sets a value for an id) is declared in game/UnkTimelineCalls.h.
// Gives the actor's component, when it has one, a key it doesn't know yet: removes the key first
// when `flag` is set, then adds it with the default mask and a delay of -1 unless fn_7100a2cfe8
// holds for it.
void fn_7100d7c828(UnkActor* self, const u64* key, bool flag);
