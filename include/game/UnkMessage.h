#pragma once

#include "types.h"

struct UnkPayload;

struct UnkMessage {
    u32 field_0x0;
    u8 pad_0x4[4];
    UnkPayload* field_0x8;
};
