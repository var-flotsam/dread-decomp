// The list at the given index of an array of owned-object lists, growing the array first when
// the index is past its end (UnkVector::GrowAt; fn_71004a972c and its copies are the same). The new
// lists are empty ones.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "game_s2/UnkOwnedList.h"

UnkOwnedList* fn_71004bf744(base::UnkVector<UnkOwnedList>* self, u32 index) { return &self->GrowAt(index); }
