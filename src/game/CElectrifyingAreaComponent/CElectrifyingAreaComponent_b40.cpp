// CElectrifyingAreaComponent unit (tools/tu_map.py unit 1236): fn_71007d2b3c tells whether the owner's
// collision container (+0x2a0) has the object named by the id at +0xec8 of the reflection singleton,
// and that object's flag at +0x6a is set. Names and layouts are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkOwner_ea {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkElectrifyingArea_ea {
    u8 pad_0x0[0x30];
    UnkOwner_ea* owner;
};

static inline const base::global::CStrId& Name_ea() {
    return *reinterpret_cast<const base::global::CStrId*>(
        reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0xec8);
}

bool fn_71007d2b3c(UnkElectrifyingArea_ea* self) {
    UnkCollisionContainer* c = self->owner->collision;
    if (!c || !fn_71008640a4(c, &Name_ea()))
        return false;
    UnkCollisionObject* o = fn_71008640a4(self->owner->collision, &Name_ea());
    return reinterpret_cast<const u8*>(o)[0x6a] != 0;
}
