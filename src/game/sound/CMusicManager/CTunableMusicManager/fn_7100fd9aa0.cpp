// sound::CMusicManager::CTunableMusicManager unit: adds an object, keyed by the string id fn_7100fed648 gives, to a
// chained hash set of objects unless one with that id is already there. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdObject_lfd9aa0;

// The objects by id.
using UnkIdSet_lfd9aa0 = base::global::CRntDictionary<CStrId, UnkIdObject_lfd9aa0*>;

// The id the object is registered under.
const CStrId* fn_7100fed648(UnkIdObject_lfd9aa0* object);

struct UnkMusicManager_lfd9aa0 {
    u8 pad_0x0[0x130];
    UnkIdSet_lfd9aa0 set;  // 0x130
};

// Declared as src/game/sound/CMusicManager/CMusicManager_b41.cpp calls it.
void fn_7100fd9aa0(char* manager, char* part) {
    auto* self = reinterpret_cast<UnkMusicManager_lfd9aa0*>(manager);
    auto* object = reinterpret_cast<UnkIdObject_lfd9aa0*>(part);
    if (!object)
        return;
    if (self->set.FindNode(*fn_7100fed648(object)))
        return;
    self->set.Insert(*fn_7100fed648(object), object);
}
