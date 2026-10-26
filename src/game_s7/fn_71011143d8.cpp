// Copies of fn_7100a3b8e0 (src/game_s4/fn_710090dbb4.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_71011143d8.
// A reflected array of callback sets, three copies (one per array type): Begin and End (an iterator
// at the first element or one past the last, its cached value re-pointed by the iterator's sync
// function), Reserve, Truncate, Erase and Insert of a moved element.
#include "game/UnkCallbackSetVector.h"

// Three copies (one per array type), each with its own sync function. End's braces matter: they
// evaluate the count before the data, as the original does.

void fn_71011143d8(UnkCallbackSetVector* v, u32 n) {
    v->Reserve(n);
}

