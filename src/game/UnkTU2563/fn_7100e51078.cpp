// Unit 2563: the move constructor of a moving magnet surface (include/game/CMinimapData.h): its id, entity
// type, path line, words and the two arrays records moved in place. Names and layouts are placeholders.
#include "game/CMinimapData.h"
#include "types.h"

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
CMinimapData::SMovingMagnetSurfaceData::SMovingMagnetSurfaceData(SMovingMagnetSurfaceData&& o)
    : sMoveEntityID(static_cast<UnkMovedStrId&&>(o.sMoveEntityID)), iEntityType(o.iEntityType),
      oPathLine(static_cast<UnkItem&&>(o.oPathLine)), field_0x60(o.field_0x60), field_0x64(o.field_0x64),
      field_0x68(o.field_0x68), field_0x6c(o.field_0x6c), field_0x70(o.field_0x70),
      field_0x78(static_cast<UnkFreedVectorPair&&>(o.field_0x78)),
      field_0xd8(static_cast<UnkFreedVectorPair&&>(o.field_0xd8)) {}
