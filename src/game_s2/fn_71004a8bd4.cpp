// Destroys the lists in an array of owned-object lists and empties it, keeping the storage (CRntVector::Clear;
// fn_71004bebec and its copies are the same).
#include "game_s2/UnkOwnedList.h"

void fn_71004a8bd4(base::global::CRntVector<UnkOwnedList>* self) { self->Clear(); }
