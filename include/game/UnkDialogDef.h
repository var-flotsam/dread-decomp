#pragma once

#include "game/UnkGameBlackboard.h"

// A property seen as a definition whose qualified id (game/UnkGameBlackboard.h) is built on first
// use (as in src/game/CGameManager.cpp).
struct UnkDialogDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};
