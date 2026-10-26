#pragma once

// The game manager singleton (reached through the GOT). Its accessors are the functions of the
// CGameManager unit (tools/tu_map.py puts fn_7100dec788, the player; fn_7100e033a8, the current
// main menu; fn_7100defcec, fn_7100defcd4, fn_7100deac84, fn_7100df5630, fn_7100de9540,
// fn_7100de94cc, fn_7100debbc8, fn_7100dffc58, fn_7100e03a78 ... in unit 2553, whose reflected
// classes are CGameManager::CTunableGameManager and its siblings), and the hand-written files
// pass it to them and read the members below. The pointer sits in base::global's .bss between
// the string pools' holders (g_unk_7101d47108, g_unk_7101d47200). Only the members the
// hand-written files touch are spelled out; the names and the layout are placeholders, and the
// object's size is unknown (0x29c0 is the largest offset read so far).

#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

// The object behind +0x608: its +0x820 is the collision tracker that fn_7100d59994,
// fn_7100d5a304, fn_7100d5a930 and fn_7100d5abfc take (game_lo/fn_71008618cc, fn_7100863a8c).
struct UnkGameManagerSub_0x608 {
    u8 pad_0x0[0x820];
    void* field_0x820;
};

// The object behind +0x1af8: entries looked up by a u16 id (fn_7100ca719c, fn_7100ca7a64;
// game_s2/fn_71002b3828), with a flag at +0xdc that stops game_s7/fn_7100aaac80.
struct UnkGameManagerSub_0x1af8 {
    u8 pad_0x0[0xdc];
    bool field_0xdc;
};

// Two arrays of string ids that share a count: the ids a lookup answers with and, at the same
// positions, the ids it matches (fn_7100dfbda4). The names are placeholders.
struct UnkGameManagerIdMap {
    base::global::CStrId* values;
    base::global::CStrId* keys;
    u32 count;

    // The answer for the first matching key, or values + count when there is none.
    base::global::CStrId* Find(const base::global::CStrId& id) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i].data == id.data)
                return values + i;
        return values + count;
    }
};

// Objects found by an id (fn_7100deba8c): the id is the u64 at +0x30 of the key object each node
// holds, hashed as its two halves xor-ed and taken modulo `capacity`.
struct UnkGameManagerKey {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkGameManagerNode {
    UnkGameManagerKey* key;
    void* value;
    u8 pad_0x10[0x10];
    UnkGameManagerNode* next;
};
struct UnkGameManagerHashMap {
    UnkGameManagerNode** buckets;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x38];
};
static_assert(sizeof(UnkGameManagerHashMap) == 0x48, "UnkGameManagerHashMap placeholder layout");

// A singly linked list of objects (+0x8 is the next link).
struct UnkGameManagerListNode {
    void* object;
    UnkGameManagerListNode* next;
};

struct UnkGameManager {
    u8 pad_0x0[0xc0];
    s32 field_0xc0;  // a mode (1 or 2) that overrides an actor's while fn_7100df5c38 holds (fn_7100bc77ec)
    s32 field_0xc4;
    s32 field_0xc8;
    u8 pad_0xcc[0x40];
    bool field_0x10c;
    u8 pad_0x10d[0x38b];
    void* field_0x498;  // an object set up through its vf_0x70 unless its vf_0x68 says it is (fn_7100b2de48)
    u8 pad_0x4a0[0x168];
    UnkGameManagerSub_0x608* field_0x608;
    u8 pad_0x610[0x358];
    CBlackboard field_0x968;  // the embedded blackboard (fn_7101047208, fn_71010472e0, fn_7101047270)
    u8 pad_0xa58[0x5a8];
    UnkGameManagerIdMap field_0x1000;  // string ids replaced by others (fn_7100dfbda4)
    u8 pad_0x1018[0x28];
    u8 field_0x1040[1];  // an embedded effect player (fn_7101053a98, fn_7101053a7c, fn_7101053a0c, fn_7101053850)
    u8 pad_0x1041[0xab7];
    UnkGameManagerSub_0x1af8* field_0x1af8;
    u8 pad_0x1b00[0x728];  // +0x2128: a callback set that fn_7100b2de48's states watch
    void* field_0x2228;    // the world (fn_7100e9b464, fn_7100e9b1f8, fn_7100e9b4b0); its +0xd63f40 is an object reference
    base::global::CStrId field_0x2230;  // the current area's name (fn_71003ad8e4)
    base::global::CStrId field_0x2238;  // an id; while it is empty fn_7100fce86c's callbacks run
    u8 pad_0x2240[0x18];
    void* field_0x2258;  // a GUI flag reference: its +0x10 holds the composition (fn_7100b2de48)
    u8 pad_0x2260[0x10];
    void* field_0x2270;    // another
    u8 pad_0x2278[0x390];  // +0x24f8: a callback set that fn_7100b2de48's states watch
    UnkGameManagerHashMap field_0x2608;  // objects by id (fn_7100deba8c)
    UnkGameManagerHashMap field_0x2650;  // more of them
    UnkGameManagerListNode* field_0x2698;  // entries whose +0x30 refers to an object with the id
    u8 pad_0x26a0[0xd8];
    UnkGameManagerListNode* field_0x2778;  // objects the lookup treats as taken
    u8 pad_0x2780[0x240];
    u8 field_0x29c0;       // a flag that blocks fn_71002fe470
};
static_assert(__builtin_offsetof(UnkGameManager, field_0x498) == 0x498, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x1000) == 0x1000, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x1040) == 0x1040, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x1af8) == 0x1af8, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x2238) == 0x2238, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x2608) == 0x2608, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x2650) == 0x2650, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x2698) == 0x2698, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x2778) == 0x2778, "UnkGameManager placeholder layout");
static_assert(__builtin_offsetof(UnkGameManager, field_0x29c0) == 0x29c0, "UnkGameManager placeholder layout");

extern UnkGameManager* g_unk_7101d47310;

// Maps `id` through field_0x1000: the id at the position of the first key equal to `id`, or `id`
// itself when no key is (src/game/CGameManager.cpp). Called with *g_unk_7101d47310, for example by
// lua_bind::GetScenarioBlackboardSectionID.
base::global::CStrId fn_7100dfbda4(UnkGameManager* self, const base::global::CStrId& id);

// Whether the object behind +0x498 (the game's mode holder) is named "EDITOR"; false when there is
// none (src/game/CGameManager.cpp; called by fn_7100c79a80).
bool fn_7100deabe4(UnkGameManager* self);

// The object fn_7100decab0 gives (also read by fn_7100e039e4); null when there is none.
struct UnkGameManagerTarget {
    u8 pad_0x0[0x248];
    void* field_0x248;
};
UnkGameManagerTarget* fn_7100decab0(UnkGameManager* self);

// Looks up the object registered under `*key` in the object behind +0x608 (the start points by
// their name's hash in fn_7100e01680); null when there is none.
void* fn_71011a3d74(UnkGameManagerSub_0x608* points, const u64* key);

// The pointer at +0x248 of the object fn_7100decab0 gives, or null when there is none
// (src/game/CGameManager.cpp; fn_71003ad014, fn_7100430830, fn_7100bda920 take it as their
// own types).
void* fn_7100defcec(UnkGameManager* self);

// Finds the object `*id` names (-1 names none): in the two maps, else, when bit 2 of `*flags` is set, in
// the list at +0x2698. An object found in the maps is dropped for the fallback when it is on the list at
// +0x2778 (unless bit 3 is set). The fallback, with bit 1 set, is fn_71011a3d74 on +0x608; else null
// (src/game/CGameManager.cpp).
void* fn_7100deba8c(UnkGameManager* self, const u64* id, const u32* flags);
void fn_7100de9540(UnkGameManager* manager, const char* name, void* object);
