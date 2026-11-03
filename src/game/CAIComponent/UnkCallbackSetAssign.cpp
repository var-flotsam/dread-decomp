// A copy of fn_7100a955cc (src/game/CCharClassWaterPoolComponent/CCharClassWaterPoolComponent_Assign.cpp)
// for the callback-set array type of unit 2702 (tools/tu_map.py; hint CAIComponent): the copy assignment
// truncates the array to nothing, reserves the source's capacity and pushes a copy of each source
// element, through this type's own Truncate, Reserve and PushBack. Names are placeholders.
#include "game/UnkCallbackSetVector.h"

struct UnkEntryVector;
struct UnkEntry;

void fn_71011146c0(UnkCallbackSetVector* v, u32 size);
void fn_7101114588(UnkEntryVector* v, const UnkEntry* value);

UnkCallbackSetVector* fn_71011142ec(UnkCallbackSetVector* self, const UnkCallbackSetVector* other) {
    if (self != other) {
        fn_71011146c0(self, 0);
        fn_71011143d8(self, other->capacity);
        for (const UnkCallbackSetElem *p = other->data, *end = p + other->count; p != end; ++p)
            fn_7101114588(reinterpret_cast<UnkEntryVector*>(self), reinterpret_cast<const UnkEntry*>(p));
    }
    return self;
}
