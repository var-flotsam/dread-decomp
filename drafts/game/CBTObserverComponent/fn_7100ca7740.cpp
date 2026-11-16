// CBTObserverComponent unit: hands the tile groups over from one setup's id to another's, switching
// on or off each group of the new setup that the old one does not also hold (all of them, last
// first, when the old one holds none). Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkTileGroup;
void fn_7100ca0b44(UnkTileGroup* self, bool enable);

// A tile group as the setup lists see it: each list keeps its groups in order of their id.
struct UnkListedTileGroup {
    u8 pad_0x0[0x10];
    u32 id;
};

using UnkTileGroupList = base::global::CRntVector<UnkListedTileGroup*>;

using UnkTileGroupDictionary = base::global::CRntDictionary<CStrId, UnkTileGroupList>;

struct UnkTileGroupsBySetup {
    u8 pad_0x0[0x8];
    UnkTileGroupDictionary groups;
};

// The node a setup's id is under, or null.
static inline UnkTileGroupDictionary::TNode* FindNode(const UnkTileGroupDictionary& groups, const CStrId& id) {
    UnkTileGroupDictionary::TNode* const* bucket = groups.BucketFor(id);
    if (!bucket)
        return nullptr;
    for (UnkTileGroupDictionary::TNode* node = *bucket; node; node = node->bucket_next)
        if (node->key == id)
            return node;
    return nullptr;
}

static inline void SetEnabled(UnkListedTileGroup* group, bool enable) {
    fn_7100ca0b44(reinterpret_cast<UnkTileGroup*>(group), enable);
}

// Whether a list holds the group (found by its place in the id order).
static inline bool Holds(const UnkTileGroupList* list, const UnkListedTileGroup* group) {
    UnkListedTileGroup** it = list->data;
    UnkListedTileGroup** end = list->data + list->count;
    while (it != end && (*it)->id < group->id)
        ++it;
    UnkListedTileGroup* found = it != end ? *it : nullptr;
    return found == group;
}

void fn_7100ca7740(UnkGameManagerSub_0x1af8* manager, const CStrId& from, const CStrId& to, bool enable) {
    UnkTileGroupsBySetup* self = reinterpret_cast<UnkTileGroupsBySetup*>(manager);
    UnkTileGroupDictionary::TNode* previous = FindNode(self->groups, from);
    UnkTileGroupDictionary::TNode* next = FindNode(self->groups, to);
    if (!next)
        return;
    UnkTileGroupList& groups = next->value;
    if (!previous) {
        for (u32 i = groups.count; i != 0; --i)
            SetEnabled(groups.data[i - 1], enable);
        return;
    }
    for (UnkListedTileGroup** it = groups.data; it != groups.data + groups.count; ++it) {
        UnkListedTileGroup* group = *it;
        if (!Holds(&previous->value, group))
            SetEnabled(group, enable);
    }
}
