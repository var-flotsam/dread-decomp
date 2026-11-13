#pragma once

// The X parasite AI component (reflected as CXParasiteAIComponent; unit 1102 of tools/tu_map.py), as far as the
// hand-written sources know it: one layout, grown by each source that reads a field it does not have yet.
// Sources that still describe this object with a struct of their own are listed in docs/JOURNAL.md
// (2026-11-13); they move here as their views are settled. Names and layouts are placeholders.
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "types.h"

// The component's table of entries at +0x14740: each entry refers to a holder, which has the entry's actor.
struct UnkXParasiteEntryHolder {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
};

struct UnkXParasiteEntryRef {
    u8 pad_0x0[0x10];
    UnkXParasiteEntryHolder* holder;  // 0x10
};

struct UnkXParasiteEntryTable {
    UnkXParasiteEntryRef** data;
    u32 count;
};

struct UnkXParasiteAI {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x146e8 - 0x1350];
    bool field_0x146e8;  // mirrored into the blackboard and the owner's timeline (fn_71006ea3fc)
    u8 pad_0x146e9[0x146f0 - 0x146e9];
    u64 field_0x146f0;
    u32 field_0x146f8;
    u8 pad_0x146fc[0x14724 - 0x146fc];
    u32 field_0x14724;
    u8 pad_0x14728[0x14740 - 0x14728];
    UnkXParasiteEntryTable entries;  // 0x14740
    u8 pad_0x14750[0x147b8 - 0x14750];
    bool field_0x147b8;
};
static_assert(__builtin_offsetof(UnkXParasiteAI, blackboard) == 0x1260, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, field_0x146e8) == 0x146e8, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, field_0x146f0) == 0x146f0, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, field_0x14724) == 0x14724, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, entries) == 0x14740, "placeholder layout");
static_assert(__builtin_offsetof(UnkXParasiteAI, field_0x147b8) == 0x147b8, "placeholder layout");
