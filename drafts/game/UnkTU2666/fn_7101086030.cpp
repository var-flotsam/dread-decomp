// Unit 2666 (Lua bindings): steps the AI component's current entry name to the next (argument
// true) or previous entry of its list, ending on the empty id past either end; with no current
// name it takes the first or last entry. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkAIStateEntry;
const CStrId* fn_7100360890(UnkAIStateEntry* entry);

// The list of entries (a count, then 4-byte-aligned pointers).
struct UnkAIStateList_l086030 {
    u32 count;  // 0x1618
    struct __attribute__((packed, aligned(4))) Entry {
        UnkAIStateEntry* p;
    } items[44];  // 0x161c

    Entry* Begin() { return items; }
    Entry* End() { return &items[count]; }
};

struct UnkAIComponent_l086030 {
    u8 pad_0x0[0x1618];
    UnkAIStateList_l086030 states;  // 0x1618
    u8 pad_0x177c[0x4];
    CStrId current;  // 0x1780
};
static_assert(__builtin_offsetof(UnkAIComponent_l086030, current) == 0x1780, "layout");

struct UnkAIOwner_l086030 {
    u8 pad_0x0[0x240];
    UnkAIComponent_l086030* component;  // 0x240
};

struct UnkAIHolder_l086030 {
    u8 pad_0x0[0x10];
    UnkAIOwner_l086030* owner;  // 0x10
};

// The manager as this function sees it: its first member.
struct UnkAIManagerView_l086030 {
    UnkAIHolder_l086030* holder;
};

static inline UnkManager_71726af468* AIManager() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

// The id's string, counting the read.
inline const CRntString& CountedString(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// Whether the entry's name is the id.
inline bool IsNamed(UnkAIStateEntry* entry, const CStrId& id) {
    const CStrId* name = fn_7100360890(entry);
    const CRntString& a = CountedString(id);
    const CRntString& b = CountedString(*name);
    return b.key == a.key;
}

// Sets the id to the entry's name.
inline void SetName(CStrId& id, UnkAIStateEntry* entry) {
    CStrId name = *fn_7100360890(entry);
    id = name;
}

// Sets the id to the entry's name, or to the empty id when there is no entry.
inline void SetNameOrClear(CStrId& id, UnkAIStateList_l086030::Entry* entry) {
    if (entry)
        SetName(id, entry->p);
    else
        id = g_unk_7101d470e0;
}

s32 fn_7101086030(lua_State* L) {
    bool forward = false;
    if (!fn_71010c0270(L, "b", &forward))
        return 0;
    UnkAIManagerView_l086030* manager = reinterpret_cast<UnkAIManagerView_l086030*>(AIManager());
    UnkAIHolder_l086030* holder = manager->holder;
    if (!holder)
        return 0;
    UnkAIOwner_l086030* owner = holder->owner;
    if (!owner)
        return 0;
    UnkAIComponent_l086030* component = owner->component;
    if (!component)
        return 0;
    UnkAIStateList_l086030& states = component->states;
    u32 count = states.count;
    if (count == 0)
        return 0;
    CStrId& current = component->current;
    if (current.data == &g_unk_7101d47108->empty) {
        SetName(current, forward ? states.items[0].p : states.items[count - 1].p);
        return 0;
    }
    if (forward) {
        for (UnkAIStateList_l086030::Entry* it = states.Begin(); it != states.End(); ++it) {
            if (IsNamed(it->p, current)) {
                UnkAIStateList_l086030::Entry* next = it + 1;
                SetNameOrClear(current, next != states.End() ? next : nullptr);
                return 0;
            }
        }
    } else {
        for (s32 i = count - 1; i >= 0; --i) {
            if (IsNamed(states.items[i].p, current)) {
                s32 previous = i - 1;
                SetNameOrClear(current, previous >= 0 ? &states.items[previous] : nullptr);
                return 0;
            }
        }
    }
    return 0;
}
