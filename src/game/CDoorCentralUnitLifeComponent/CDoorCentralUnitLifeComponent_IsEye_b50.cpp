// CDoorCentralUnitLifeComponent unit (tools/tu_map.py, the name from reflection): whether `part` is
// named "eye" (its string id at +0x20 against a function-local static id). Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkDoorPart {
    u8 unk_0x0[0x20];
    UnkStringData* name;  // 0x20
};

bool fn_71008ca968(void*, const UnkDoorPart* part) {
    static CStrId s_eye("eye", true);
    return part && part->name == s_eye.data;
}
