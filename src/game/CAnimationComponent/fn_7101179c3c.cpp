// What fn_710002fb70 reports for the part at +0xb0 of the object of the track whose name hash is
// `key`, or false when there is no such entry or object; the lookup is fn_7101177cac's
// (src/game/CAnimationComponent/fn_710117af10.cpp).
#include "game/UnkAnimationComponent.h"
#include "types.h"

bool fn_710002fb70(void* timer);

bool fn_7101179c3c(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return false;
    UnkTrackEntry* entry = *it;
    if (entry && entry->field_0x70)
        return fn_710002fb70(reinterpret_cast<char*>(entry->field_0x70) + 0xb0);
    return false;
}
