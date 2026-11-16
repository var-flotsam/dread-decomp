// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): the destructor of a static
// array of nine string ids (its callers register it with atexit): releases them last to first.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::UnkStrIdMember;

struct UnkIdArray9_e1548 {
    UnkStrIdMember ids[9];
};

void fn_7100ee1548(void* self) {
    UnkIdArray9_e1548* array = static_cast<UnkIdArray9_e1548*>(self);
    for (int i = 9; i-- > 0;)
        array->ids[i].~UnkStrIdMember();
}
