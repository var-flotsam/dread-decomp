// Destroys the lists in an array of owned-object lists and frees its storage (CRntVector::Free;
// fn_71004a8f10 and its copies are the same).
#include "game_s2/UnkOwnedList.h"

void fn_71004bef28(base::global::CRntVector<UnkOwnedList>* self) { self->Free(); }
