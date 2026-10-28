// Copies of fn_7100a3bfe8 (src/game/CCharClassWaterPoolComponent/fn_710090dbb4.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7101114ae0.
// A reflected array of callback sets, three copies (one per array type): Begin and End (an iterator
// at the first element or one past the last, its cached value re-pointed by the iterator's sync
// function), Reserve, Truncate, Erase and Insert of a moved element.
#include "game/UnkCallbackSetVector.h"

// Three copies (one per array type), each with its own sync function. End's braces matter: they
// evaluate the count before the data, as the original does.

void fn_7101114ae0(UnkCallbackSetVector* v, UnkCallbackSetElem* value, u32 index) {
    v->Insert<fn_71011143d8>(static_cast<UnkCallbackSetElem&&>(*value), index);
}

