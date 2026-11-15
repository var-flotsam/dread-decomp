// Move assignment of an array of owned-object lists (fn_71004a9050 and its copies are the same):
// destroys the lists it holds, then takes over the other array's storage.
#include "game_s2/UnkOwnedList.h"

base::global::CRntVector<UnkOwnedList>* fn_71004bf068(base::global::CRntVector<UnkOwnedList>* self,
                                              base::global::CRntVector<UnkOwnedList>* other) {
    return &(*self = static_cast<base::global::CRntVector<UnkOwnedList>&&>(*other));
}
