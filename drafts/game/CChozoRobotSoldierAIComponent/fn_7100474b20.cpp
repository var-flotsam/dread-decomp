// CChozoRobotSoldierAIComponent unit: whether the owner's target is blocked by one of the entries
// fn_7100479420 lists: an entry closer to the owner than 150 units (flat), or one not in front of the
// owner as seen towards the target. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEntryHolder_l474b20 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
};

struct UnkEntry_l474b20 {
    u8 pad_0x0[0x10];
    UnkEntryHolder_l474b20* holder;  // 0x10
};

struct UnkEntryList_l474b20 {
    UnkEntry_l474b20** data;
    u32 count;
};

UnkEntryList_l474b20* fn_7100479420();

struct UnkChozoRobotSoldierAI_l474b20 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

// An actor's position (its transform's translation column).
inline CVector3D Pos_l474b20(const UnkActor* actor) {
    return CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

inline UnkEntryHolder_l474b20* Holder_l474b20(UnkEntry_l474b20* entry) {
    return entry ? entry->holder : nullptr;
}

// The position of a holder's actor.
inline CVector3D Pos_l474b20(const UnkEntryHolder_l474b20* holder) {
    return Pos_l474b20(holder->actor);
}

bool fn_7100474b20(UnkChozoRobotSoldierAI_l474b20* self) {
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return false;
    CVector2D to_target;
    CVector3D offset = Pos_l474b20(target) - Pos_l474b20(self->owner);
    fn_710007b430(&to_target, &offset);
    UnkEntryList_l474b20* list = fn_7100479420();
    UnkEntry_l474b20** end = list->data + list->count;
    for (UnkEntry_l474b20** it = list->data; it != end; ++it) {
        CVector2D to_entry;
        offset = Pos_l474b20(Holder_l474b20(*it)) - Pos_l474b20(self->owner);
        fn_710007b430(&to_entry, &offset);
        if (Length(to_entry) < 150.0f)
            return true;
        if (!(Dot(to_target, to_entry) > 0.0f))
            return true;
    }
    return false;
}
