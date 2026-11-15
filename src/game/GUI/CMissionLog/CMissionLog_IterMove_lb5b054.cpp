// CMissionLog's unit (tools/tu_map.py): the move helper of the iterator value over a reflected array of
// UnkMissionLogElem. Names and layouts are placeholders.
#include "game/GUI/CMissionLog/UnkMissionLogElem.h"
#include "base/UnkCollectionIter.h"

void fn_7100b5b054(void* dst, const void* src) {
    base::refl_IterValueMove<UnkMissionLogElem>(dst, src);
}
