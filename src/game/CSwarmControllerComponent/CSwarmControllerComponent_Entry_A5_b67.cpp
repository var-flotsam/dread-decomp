// CSwarmControllerComponent's unit (tools/tu_map.py): returns a copy of entry `index` of the table reached through
// +0x1e0 (entries of 0x18 at +0x2e8: a string id, a file-path id and a counted reference, count at +0x54). Names are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkCountedRef_A5_b67.h"
#include "game/UnkCounted_A5_b67.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSwarmEntry_A5_b67 {
    CStrId name;
    CFilePathStrId path;
    UnkCountedRef_A5_b67 ref;
};

struct UnkSwarmTable_A5_b67 {
    u8 pad_0x0[0x2e8];
    UnkSwarmEntry_A5_b67 entries[1];
};

struct UnkSwarmController_A5_b67 {
    u8 pad_0x0[0x1e0];
    UnkSwarmTable_A5_b67* table;
};

UnkSwarmEntry_A5_b67 CSwarmControllerComponent_GetCharClassEntry(UnkSwarmController_A5_b67* self, s32 index) {
    return self->table->entries[index];
}
