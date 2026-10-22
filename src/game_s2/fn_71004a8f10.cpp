// Destroys the lists in an array of owned-object lists and frees its storage (UnkVector::Free;
// fn_71004bef28 and its copies are the same).
#include "game_s2/UnkOwnedList.h"

void fn_71004a8f10(base::UnkVector<UnkOwnedList>* self) { self->Free(); }
