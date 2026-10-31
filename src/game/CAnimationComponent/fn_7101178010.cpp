// Passes a flag to the object of the track whose name hash is `key` (fn_710070d884), if there is such
// an entry with an object; the lookup is fn_7101177cac's (src/game/CAnimationComponent/fn_710117af10.cpp).
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkTrackObject_7101178010;
void fn_710070d884(UnkTrackObject_7101178010* obj, bool flag);

void fn_7101178010(UnkAnimationComponent* self, u64 key, bool flag) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return;
    UnkTrackEntry* entry = *it;
    if (entry && entry->field_0x70)
        fn_710070d884(reinterpret_cast<UnkTrackObject_7101178010*>(entry->field_0x70), flag);
}
