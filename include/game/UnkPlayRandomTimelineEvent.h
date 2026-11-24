#pragma once

#include "base/global/CStrId.h"
#include "game/UnkTimelineEventParams_lbc844c.h"
#include "types.h"

// The event: the timeline event base (vtable and one word), a block of four words and eight string ids.
struct UnkPlayRandomTimelineEvent {
    void** vtable;
    u32 field_0x8;
    UnkTimelineEventParams_lbc844c params;  // 0xc
    base::global::CStrId sName0;
    base::global::CStrId sName1;
    base::global::CStrId sName2;
    base::global::CStrId sName3;
    base::global::CStrId sName4;
    base::global::CStrId sName5;
    base::global::CStrId sName6;
    base::global::CStrId sName7;
};
