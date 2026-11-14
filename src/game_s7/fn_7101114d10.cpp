// Copies of fn_7100a3c218 (src/game/CCharClassWaterPoolComponent/fn_710090dbb4.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7101114d10.
// A reflected array of callback sets, three copies (one per array type): Begin and End (an iterator
// at the first element or one past the last, its cached value re-pointed by the iterator's sync
// function), Reserve, Truncate, Erase and Insert of a moved element.
#include "base/UnkErasedVector.h"
#include "base/UnkVector.h"
#include "game/CChangeStateDoorsLogicAction.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/UnkCallbackSetVector.h"
#include "game/UnkTU1601.h"

// Three copies (one per array type), each with its own sync function. End's braces matter: they
// evaluate the count before the data, as the original does.

void fn_7101114d10(UnkCallbackSetVector* v, u32 index) {
    v->Erase(index);
}
