// Moves a 12-byte element into a growable array at the given index, shifting the elements after it up.
#include "game_s2/UnkItem12Vector.h"

void fn_71003ee494(base::UnkVector<UnkItem12>* self, UnkItem12* value, u32 index) {
    InsertItem12(self, value, index);
}
