// Loops over the tracks of the component's set: adding `id` to (fn_710002fd10) or removing it from
// (fn_710002ffb4) the key list at +0xb0 of every track (fn_710117ab64), and whether any playing track
// reports what fn_710070d820 tests (fn_710117ad58); and that test for one track (fn_710117adc0). The end is reread after each call.
#include "game/UnkAnimationComponent.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

static inline UnkGuiKeyList* KeyList(UnkTrackEntry* entry) {
    return reinterpret_cast<UnkGuiKeyList*>(reinterpret_cast<char*>(entry->field_0x70) + 0xb0);
}

void fn_710117ab64(UnkAnimationComponent* self, bool add, base::global::CStrId* id) {
    for (UnkTrackEntry** it = self->field_0x40->field_0xc8.values; it != self->field_0x40->field_0xc8.End(); ++it) {
        if (add)
            fn_710002fd10(KeyList(*it), id);
        else
            fn_710002ffb4(KeyList(*it), id);
    }
}

struct UnkTrackObject_710117adc0;
bool fn_710070d820(UnkTrackObject_710117adc0* obj);

bool fn_710117ad58(UnkAnimationComponent* self) {
    for (UnkTrackEntry** it = self->field_0x40->field_0xf0.values; it != self->field_0x40->field_0xf0.End(); ++it)
        if (fn_710070d820(reinterpret_cast<UnkTrackObject_710117adc0*>((*it)->field_0x70)))
            return true;
    return false;
}

// What fn_710070d820 reports for the object of the track whose name hash is `key` (even a null one),
// or false when there is no such entry; the lookup is fn_7101177cac's
// (src/game/CAnimationComponent/fn_710117af10.cpp).
bool fn_710117adc0(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return false;
    UnkTrackEntry* entry = *it;
    if (!entry)
        return false;
    return fn_710070d820(reinterpret_cast<UnkTrackObject_710117adc0*>(entry->field_0x70));
}
