#pragma once

#include "game/UnkSoundIdRef.h"

struct UnkSoundIdListNode {
    u32 key;
    UnkSoundIdRef ref;
    UnkSoundIdListNode* next;
};
