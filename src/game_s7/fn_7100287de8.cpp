// Copies of fn_71002b1a3c (src/game_s2/fn_71002b1a3c.cpp), which the binary
// repeats at other addresses: fn_7100287de8, fn_7100738c9c, fn_710073d630.
// The list at the given index of an array of owned-object lists, growing the array first when
// the index is past its end (UnkVector::GrowAt; fn_71004a972c and its copies are the same). The new
// lists are empty ones.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "game_s2/UnkOwnedList.h"

UnkOwnedList* fn_7100287de8(base::UnkVector<UnkOwnedList>* self, u32 index) { return &self->GrowAt(index); }

UnkOwnedList* fn_7100738c9c(base::UnkVector<UnkOwnedList>* self, u32 index) { return &self->GrowAt(index); }

UnkOwnedList* fn_710073d630(base::UnkVector<UnkOwnedList>* self, u32 index) { return &self->GrowAt(index); }

