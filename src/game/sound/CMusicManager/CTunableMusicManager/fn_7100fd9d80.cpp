// sound::CMusicManager::CTunableMusicManager unit: adds an object, keyed by the string id at its start, to a
// chained hash set of objects unless one with that id is already there. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdObject_lfd9d80 {
    CStrId id;
};

// The objects by id.
using UnkIdSet_lfd9d80 = base::global::CRntDictionary<CStrId, UnkIdObject_lfd9d80*>;

struct UnkMusicManager_lfd9d80 {
    u8 pad_0x0[0x178];
    UnkIdSet_lfd9d80 set;  // 0x178
};

// Declared as src/game/sound/CMusicManager/CMusicManager_b41.cpp calls it.
void fn_7100fd9d80(char* manager, char* object) {
    auto* self = reinterpret_cast<UnkMusicManager_lfd9d80*>(manager);
    if (!object)
        return;
    const CStrId& id = reinterpret_cast<UnkIdObject_lfd9d80*>(object)->id;
    if (self->set.FindNode(id))
        return;
    self->set.Insert(id, reinterpret_cast<UnkIdObject_lfd9d80*>(object));
}
