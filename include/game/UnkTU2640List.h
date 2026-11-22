#pragma once

#include "game/UnkTU2640Node.h"

struct UnkTU2640List {
    u8 pad_0x0[0x40];
    UnkTU2640Node* head;
};
