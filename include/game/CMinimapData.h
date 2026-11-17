#pragma once

// CMinimapData (a reflected asset class, 0x6358 bytes; unit 2563 of tools/tu_map.py). Only what sources need of
// it so far: the record of its aMovingMagnetSurfaces vector. Names from reflection and
// mercury-engine-data-structures; the rest are placeholders.
#include "base/UnkFreedVectorPair.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

struct CMinimapData {
    struct SMovingMagnetSurfaceData;
};

// An element of aMovingMagnetSurfaces (0x138 bytes). The first three members are the reflected ones; after
// +0x60 the reflected fields (bInverseNormal, fSurfaceLenght, vSurfaceStart, vSurfaceEnd) do not line up with
// what the code copies (+0x74 is never copied), so those keep placeholders, as do the two unreflected records.
struct CMinimapData::SMovingMagnetSurfaceData {
    UnkMovedStrId sMoveEntityID;     // 0x0
    u32 iEntityType;                 // 0x8
    UnkItem oPathLine;               // 0x10 (an SLogicSubPath by its reflection: a vtable and its nodes)
    u8 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    UnkFreedVectorPair field_0x78;
    UnkFreedVectorPair field_0xd8;

    SMovingMagnetSurfaceData(SMovingMagnetSurfaceData&& other);       // fn_7100e51078
    SMovingMagnetSurfaceData(const SMovingMagnetSurfaceData& other);  // fn_7100e50e20
    ~SMovingMagnetSurfaceData();                                       // fn_7100e4e310
};
static_assert(sizeof(CMinimapData::SMovingMagnetSurfaceData) == 0x138, "SMovingMagnetSurfaceData layout");
static_assert(__builtin_offsetof(CMinimapData::SMovingMagnetSurfaceData, field_0x78) == 0x78,
              "SMovingMagnetSurfaceData layout");
