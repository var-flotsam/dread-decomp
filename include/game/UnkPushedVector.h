#pragma once

#include "game/CGoliathXBurstProjectionBombMovement.h"
#include "game/UnkCopiedVector.h"

struct UnkPushedVector : UnkCopiedVector<UnkValues3> {
    UnkPushedVector& operator=(const UnkPushedVector& other);
};
