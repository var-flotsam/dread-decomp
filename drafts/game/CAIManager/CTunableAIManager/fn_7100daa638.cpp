// CTunableAIManager's unit (tools/tu_map.py): records a named position in the manager's lists, one
// list per key: an entry with the name is updated, else one is appended (the list is made first when
// the key has none). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::math::CVector3D;

// A name and the position recorded for it.
struct UnkNamedPos_daa638 {
    CStrId name;
    CVector3D pos;

    UnkNamedPos_daa638(const CStrId& name, const CVector3D& pos) : name(name), pos(pos) {}
};
static_assert(sizeof(UnkNamedPos_daa638) == 0x18, "layout");

using UnkNamedPosList_daa638 = CRntVector<UnkNamedPos_daa638>;

// Grows the list to at least n entries (its Reserve).
void fn_7100dac000(UnkNamedPosList_daa638* self, u32 n);

// The lists by key: values and keys in two parallel arrays.
struct UnkNamedPosMap_daa638 {
    UnkNamedPosList_daa638* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 step;

    // The key's list, or End() when it has none.
    UnkNamedPosList_daa638* Find(u32 key) {
        for (u64 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }
    UnkNamedPosList_daa638* End() { return &values[count]; }
    UnkNamedPosList_daa638& operator[](u32 key) {
        u64 i = 0;
        for (; i < count; ++i) {
            if (keys[i] == key)
                break;
        }
        return values[i];
    }
    void Add(u32 key, UnkNamedPosList_daa638&& list);
};

// Grows the map to the given capacity.
void fn_7100dabed0(UnkNamedPosMap_daa638* self, u32 capacity);

inline void UnkNamedPosMap_daa638::Add(u32 key, UnkNamedPosList_daa638&& list) {
    if (count == capacity)
        fn_7100dabed0(this, count + step);
    keys[count] = key;
    new (&values[count]) UnkNamedPosList_daa638(static_cast<UnkNamedPosList_daa638&&>(list));
    ++count;
}

struct UnkAIManagerPositions_daa638 {
    u8 pad_0x0[0xbc8];
    UnkNamedPosMap_daa638 positions;
};

// Appends an entry, growing the list to the next multiple of its step when it is full (a list
// with no step hangs, as in the original).
static inline void Append(UnkNamedPosList_daa638& list, UnkNamedPos_daa638&& entry) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100dac000(&list, new_capacity);
    UnkNamedPos_daa638* slot = &list.data[list.count];
    CStrId::MoveConstructReloading(&slot->name, entry.name);
    slot->pos = entry.pos;
    ++list.count;
}

void fn_7100daa638(UnkManager_71726af468* self, CStrId name, u32 value, const CVector3D& pos) {
    UnkNamedPosMap_daa638& map = reinterpret_cast<UnkAIManagerPositions_daa638*>(self)->positions;
    if (map.Find(value) == map.End())
        map.Add(value, UnkNamedPosList_daa638(0xf));

    UnkNamedPosList_daa638& list = map[value];
    bool found = false;
    for (UnkNamedPos_daa638 *entry = list.data, *end = entry + list.count; entry != end; ++entry) {
        if (entry->name == name) {
            entry->pos = pos;
            found = true;
        }
    }
    if (found)
        return;
    Append(map[value], UnkNamedPos_daa638(name, pos));
}
