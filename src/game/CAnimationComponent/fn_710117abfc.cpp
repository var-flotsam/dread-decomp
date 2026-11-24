// Adds `id` to (fn_710002fd10) or removes it from (fn_710002ffb4) the key list at +0xb0 of the object
// of the track whose name hash is `key`, if there is such an entry with an object; the lookup is
// fn_7101177cac's (src/game/CAnimationComponent/fn_710117af10.cpp).
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

// The track object as this function reads it: the key list at +0xb0.
struct UnkTrackKeys_710117abfc {
    u8 pad_0x0[0xb0];
    UnkGuiKeyList field_0xb0;  // 0xb0
};

void fn_710117abfc(UnkAnimationComponent* self, bool add, base::global::CStrId* id, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return;
    UnkTrackEntry* entry = *it;
    if (!entry || !entry->field_0x70)
        return;
    auto* track = reinterpret_cast<UnkTrackKeys_710117abfc*>(entry->field_0x70);
    UnkGuiKeyList* list = &track->field_0xb0;
    if (add)
        fn_710002fd10(list, id);
    else
        fn_710002ffb4(list, id);
}
