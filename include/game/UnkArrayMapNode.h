#pragma once

#include "base/global/CRntVector.h"
#include "game/UnkMapElem40.h"
#include "game/UnkMapKey.h"
#include "types.h"

struct UnkArrayMapNode {
    UnkMapKey* key;
    base::global::CRntVector<UnkMapElem40> value;
    u8 unk_0x30[0x10];
    UnkArrayMapNode* next;
};
