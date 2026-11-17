// Unit 2666 Lua binding fn_7101090450: finds an object by name (the game manager, else the GUI), reads the
// colour of one of its slots, overrides the components the script gave (non-negative ones) and writes it back.
// Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSlots.h"
#include "game/UnkTU0.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2730.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

// The part of the object fn_7100233684 returns that holds the slots.
struct UnkSlotOwner_l090450 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // 0x138
};

struct UnkSlotObject_l090450 {
    u8 pad_0x0[0x50];
    UnkSlotOwner_l090450* owner;  // 0x50
};

// The object the game manager knows under a name (the name's CRC64 and the kind g_unk_71727bebb8).
static inline void* FindByName(UnkGameManager* manager, const char* name) {
    u32 kind_value = g_unk_71727bebb8;
    u64 hash;
    alignas(8) u32 kind;
    hash = fn_7100001570(name, __builtin_strlen(name));
    kind = kind_value;
    return fn_7100deba8c(manager, &hash, &kind);
}

// The slot kind a script names: 1 and 2 for two ids of the table, 0 for anything else.
static inline u32 SlotKind(const CStrId& id) {
    // The table is asked for once more first, for nothing.
    GameManagerIdTable();
    u32 kind = id == *reinterpret_cast<const CStrId*>(&GameManagerIdTable().pad_0x0[0xc48]) ? 1 : 0;
    if (id == *reinterpret_cast<const CStrId*>(&GameManagerIdTable().pad_0x0[0xc50]))
        kind = 2;
    return kind;
}

s32 fn_7101090450(lua_State* L) {
    const char* name;
    const char* slot;
    const char* kind_name;
    f32 r = -1.0f;
    f32 g = -1.0f;
    f32 b = -1.0f;
    f32 a = -1.0f;
    if (!fn_71010c0270(L, "sssffff", &name, &slot, &kind_name, &r, &g, &b, &a))
        return 0;

    void* object = FindByName(g_unk_7101d47310, name);
    if (!object) {
        void* gui = g_unk_7101d47330;
        CStrId id(name, true);
        object = fn_71011986a8(gui, &id);
    }
    if (!object)
        return 0;
    UnkSlotObject_l090450* inner =
        reinterpret_cast<UnkSlotObject_l090450*>(fn_7100233684(static_cast<UnkStruct_7100233684*>(object)));
    if (!inner)
        return 0;
    UnkSlotOwner_l090450* owner = inner->owner;
    if (!owner)
        return 0;

    CStrId kind_id(kind_name, true);
    u32 kind = SlotKind(kind_id);
    CColor4F color(1.0f, 1.0f, 1.0f, 1.0f);
    u64 key = fn_7100001570(slot, __builtin_strlen(slot));
    fn_71001c2bac(owner->slots, key, kind, &color, 1);
    if (r >= 0.0f)
        color.r = r <= 0.0f ? 0.0f : r;
    if (g >= 0.0f)
        color.g = g <= 0.0f ? 0.0f : g;
    if (b >= 0.0f)
        color.b = b <= 0.0f ? 0.0f : b;
    if (a >= 0.0f)
        color.a = a <= 0.0f ? 0.0f : a;
    fn_71001c28f8(owner->slots, key, kind, &color, 1);
    return 0;
}
