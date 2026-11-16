// CAIComponent unit (tools/tu_map.py unit 2702): inserts a copy of a callback set element at an index of
// the array, moving the later elements up one place (appending is fn_7101114588; the moved-element insert
// is fn_7101114ae0). Names and layouts are placeholders.
#include <new>

#include "game/UnkCallbackSetVector.h"
#include "types.h"

void fn_7101114588(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);

void fn_7101114828(UnkCallbackSetVector* v, const UnkCallbackSetElem* value, u32 index) {
    u32 n = v->count;
    if (n == index) {
        fn_7101114588(v, value);
        return;
    }
    fn_71011143d8(v, v->GrownCapacity());
    // The elements from the last down to the index move up one place.
    for (u32 i = v->count - 1; i >= index; --i) {
        UnkCallbackSetElem* from = &v->data[i];
        new (from + 1) UnkCallbackSetElem(static_cast<UnkCallbackSetElem&&>(*from));
        v->data[i].~UnkCallbackSetElem();
        if (i == 0)
            break;
    }
    new (&v->data[index]) UnkCallbackSetElem(*value);
    ++v->count;
}
