#pragma once

#include "game/UnkActorLink_l7a58c0.h"
#include "game/UnkCutsceneList_l7a58c0.h"
#include "types.h"

// The 0x88-byte entry the cutscene component keeps per actor.
struct UnkCutsceneEntry_l7a58c0 {
    virtual void vf_0x0();
    base::global::CRntVector<u8> flags;  // 0x8: one per place in the list
    base::global::CStrId name;                         // 0x30
    UnkActorLink_l7a58c0 link;           // 0x38
    UnkCutsceneList_l7a58c0* list;       // 0x78
    u8 field_0x80;
    u8 field_0x81;
};
