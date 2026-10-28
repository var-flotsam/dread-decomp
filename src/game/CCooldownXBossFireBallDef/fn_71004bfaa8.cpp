// End of an array of owned-object lists (base::refl_End): the iterator one past the last list.
#include "base/UnkCollectionIter.h"
#include "game_s2/UnkOwnedList.h"

using UnkListIter = base::UnkCollectionIter<UnkOwnedList>;

void fn_71004bfaa8(const base::UnkVector<UnkOwnedList>* v, UnkListIter* out) { *out = UnkListIter(v->count, v->data + v->count); }
