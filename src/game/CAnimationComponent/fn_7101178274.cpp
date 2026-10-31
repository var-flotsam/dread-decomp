// Calls fn_710070d864 on the object of the track whose name hash is `key`, if there is such an entry
// with an object; the lookup is fn_7101177cac's (src/game/CAnimationComponent/fn_710117af10.cpp).
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkTrackObject_7101178274;
void fn_710070d864(UnkTrackObject_7101178274* obj);

void fn_7101178274(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return;
    UnkTrackEntry* entry = *it;
    if (entry && entry->field_0x70)
        fn_710070d864(reinterpret_cast<UnkTrackObject_7101178274*>(entry->field_0x70));
}
