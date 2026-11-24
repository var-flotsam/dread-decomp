#pragma once

#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkModelData;

struct UnkModel {
    u8 pad_0x0[0x8];
    UnkSceneNode field_0x8;  // the scene node, after the vtable; the render manager puts it on a layer
    char field_0x20[1];  // set up by fn_710020d32c after construction
    u8 pad_0x21[0x117];
    UnkModelData* field_0x138;
};
