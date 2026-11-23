#pragma once

#include "types.h"

struct UnkWidgetMember {
    u8 data[0x38];
    UnkWidgetMember() {}
    UnkWidgetMember(const UnkWidgetMember&) {}
};
