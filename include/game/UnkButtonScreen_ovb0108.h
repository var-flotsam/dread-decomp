#pragma once

#include "game/UnkButtonList_ovb0108.h"
#include "game/UnkTU2744.h"

struct UnkButtonScreen_ovb0108 {
    u8 pad_0x0[0x250];
    UnkButtonList_ovb0108* list;           // 0x250
    UnkButtonTemplate_ovb0108* button_def;  // 0x258
};
