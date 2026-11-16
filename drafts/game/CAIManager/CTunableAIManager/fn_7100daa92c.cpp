// CTunableAIManager unit: removes the point named `name` from the point list stored under `key` in the
// manager's keyed table of named points (+0xbc8), if both are there. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkNamedPoint_b0025;
void fn_7100dac138(base::global::CRntVector<UnkNamedPoint_b0025>* self, u32 index);

struct UnkPoint_ldaa92c {
    CStrId name;
    f32 x, y, z;
    u32 unk_0x14;
};

struct UnkPointMap_ldaa92c {
    CRntVector<UnkPoint_ldaa92c>* values;
    u32* keys;
    u32 count;

    CRntVector<UnkPoint_ldaa92c>* Find(u32 key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }
    bool Contains(u32 key) { return Find(key) != &values[count]; }
    CRntVector<UnkPoint_ldaa92c>& operator[](u32 key) { return *Find(key); }
};

struct UnkAIManager_ldaa92c {
    u8 pad_0x0[0xbc8];
    UnkPointMap_ldaa92c points;  // 0xbc8
};

static inline UnkPoint_ldaa92c* End(CRntVector<UnkPoint_ldaa92c>& list) { return list.data + list.count; }

static inline UnkPoint_ldaa92c* FindPoint(CRntVector<UnkPoint_ldaa92c>& list, const CStrId& name) {
    UnkPoint_ldaa92c* end = End(list);
    for (UnkPoint_ldaa92c* it = list.data; it != end; ++it) {
        if (it->name.data == name.data)
            return it;
    }
    return end;
}

void fn_7100daa92c(UnkManager_71726af468* manager, CStrId name, u32 key) {
    UnkPointMap_ldaa92c& points = reinterpret_cast<UnkAIManager_ldaa92c*>(manager)->points;
    if (!points.Contains(key))
        return;
    UnkPoint_ldaa92c* it = FindPoint(points[key], name);
    if (it == End(points[key]))
        return;
    CRntVector<UnkPoint_ldaa92c>& list = points[key];
    fn_7100dac138(reinterpret_cast<CRntVector<UnkNamedPoint_b0025>*>(&list), it - list.data);
}
