// Unit 140 of tools/tu_map.py (0x710014803c-0x7100149bec, no reflected class): the scene object of
// game/UnkSceneObject.h. Holds fn_7100148178, which hands a draw list to one of the five slots of
// the sub-object at +0x130 (fn_71001c0424). Names and layouts are placeholders.
#include "game/UnkSceneObject.h"

bool fn_71001c0424(UnkSceneObjectSub* sub, u32 slot, const UnkSceneSpan& list, u32 flag);

bool fn_7100148178(UnkSceneObject* self, u32 slot, const UnkSceneSpan& list, u32 flag) {
    if (slot < 5)
        return fn_71001c0424(self->field_0x130, slot, list, flag);
    return false;
}
