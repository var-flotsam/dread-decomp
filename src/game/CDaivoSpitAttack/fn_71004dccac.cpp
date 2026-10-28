// Appends a copy of a 0x50-byte element to a growable array of them. When the array has to grow,
// the elements are moved over one by one: the moved-from sub-object is cleared and the old
// element is destroyed through its vtable (slot 1).
#include "game_s2/UnkItem50.h"

// NON_MATCHING(minor): the three zero stores of the moved element (0x34, 0x3c, 0x44) are emitted in
// ascending order, the original has the first and last swapped
void fn_71004dccac(base::UnkVector<UnkItem>* self, const UnkItem* value) {
    self->PushBackCopy(*value);
}
