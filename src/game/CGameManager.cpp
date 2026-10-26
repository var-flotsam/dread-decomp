// The game manager (CGameManager): the lookup that replaces a string id by another.
#include "game/UnkGameManager.h"

using base::global::CStrId;

CStrId fn_7100dfbda4(UnkGameManager* self, const CStrId& id) {
    CStrId* it = self->field_0x1000.Find(id);
    if (it != self->field_0x1000.values + self->field_0x1000.count)
        return *it;
    return id;
}
