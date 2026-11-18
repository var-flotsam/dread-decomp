// base::tunable::CTunable's unit: the entry of a map from string ids to owned lists (two parallel
// arrays, as in fn_71001358e0), added empty when the id is not in it; returns the entry.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CStrId;

using UnkListMap133a94 = base::global::CRntSmallDictionary<CStrId, UnkOwnedList>;

void fn_71001358e0(UnkListMap133a94* self, u32 capacity);

UnkOwnedList* fn_7100133a94(UnkListMap133a94* self, const CStrId* key) {
    UnkOwnedList* found = self->Find(*key);
    if (found != self->End())
        return found;
    UnkOwnedList empty;
    if (self->count == self->capacity)
        fn_71001358e0(self, self->count + self->step);
    new (&self->keys[self->count]) CStrId(*key);
    UnkOwnedList* slot = &self->values[self->count];
    new (slot) UnkOwnedList(static_cast<UnkOwnedList&&>(empty));
    self->count++;
    UnkOwnedList* last = &self->values[self->count] - 1;
    return self->count ? last : self->values;
}
