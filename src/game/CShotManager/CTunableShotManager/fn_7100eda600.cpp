// Constructor of a block of eight file-path ids (the slot type fn_7100edce2c resets), each set
// to the pool's empty id, creating the pool first if needed.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "game/UnkEmptyPathId.h"
#include "game/UnkPathIds_0x40.h"
#include "types.h"

using base::global::UnkStringData;

void fn_7100eda600(UnkPathIds_0x40* self) {
    new (self) UnkPathIds_0x40;
}
