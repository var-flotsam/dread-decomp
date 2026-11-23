#pragma once

#include "base/global/CStrId.h"
#include "game/UnkTimelineEventParams_lbc844c.h"
#include "types.h"

// The event: the timeline event base (vtable and one word), a block of four words and eight string ids.
struct UnkPlayRandomTimelineEvent {
    void** vtable;
    u32 field_0x8;
    UnkTimelineEventParams_lbc844c params;  // 0xc
    base::global::CStrId field_0x20;
    base::global::CStrId field_0x28;
    base::global::CStrId field_0x30;
    base::global::CStrId field_0x38;
    base::global::CStrId field_0x40;
    base::global::CStrId field_0x48;
    base::global::CStrId field_0x50;
    base::global::CStrId field_0x58;
};
