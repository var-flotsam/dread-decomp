// Begin of an array of owned-object lists (base::refl_Begin): the iterator at index 0.
#include "base/UnkCollectionIter.h"
#include "game_s2/UnkOwnedList.h"

using UnkListIter = base::UnkCollectionIter<UnkOwnedList>;

void fn_71004a9990(const base::UnkVector<UnkOwnedList>* v, UnkListIter* out) { *out = UnkListIter(0, v->data); }
