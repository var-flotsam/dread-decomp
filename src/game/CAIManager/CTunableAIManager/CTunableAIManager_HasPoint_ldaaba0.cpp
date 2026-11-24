// CTunableAIManager unit: whether the point list stored under `key` in the manager's keyed table of named
// points (+0xbc8) holds a point named `name`. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkPoint_ldaaba0 {
    CStrId name;
    f32 x, y, z;
    u32 unk_0x14;
};

struct UnkPointMap_ldaaba0 {
    CRntVector<UnkPoint_ldaaba0>* values;
    u32* keys;
    u32 count;

    CRntVector<UnkPoint_ldaaba0>* Find(u32 key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }
    bool Contains(u32 key) { return Find(key) != &values[count]; }
    CRntVector<UnkPoint_ldaaba0>& operator[](u32 key) { return *Find(key); }
};

struct UnkAIManager_ldaaba0 {
    u8 pad_0x0[0xbc8];
    UnkPointMap_ldaaba0 points;  // 0xbc8
};

static inline bool SameId(CStrId id, const CStrId& other) { return other.data == id.data; }

bool CTunableAIManager_KeyedListHasName(UnkManager_71726af468* manager, u32 key, CStrId name) {
    UnkPointMap_ldaaba0& points = reinterpret_cast<UnkAIManager_ldaaba0*>(manager)->points;
    if (!points.Contains(key) || points[key].count == 0)
        return false;
    CRntVector<UnkPoint_ldaaba0>& list = points[key];
    for (UnkPoint_ldaaba0 *it = list.data, *end = list.data + list.count; it != end; ++it) {
        if (SameId(it->name, name))
            return true;
    }
    return false;
}
