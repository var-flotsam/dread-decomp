#pragma once

#include "game/UnkElemData2661.h"
#include "game/UnkTU2661.h"

struct UnkElem2661 {
    char* vtable;
    UnkElemData2661 data;  // 0x8

    UnkElem2661(UnkElem2661&& other) : vtable((char*)g_unk_7101cac988 + 0x10), data(other.data) {}
};
