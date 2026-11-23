#pragma once

#include "base/global/CRntString.h"
#include "types.h"

// The pooled data behind a string id as its text accessor sees it: the text as a CRntString at
// +0x10 and a count of the reads at +0x44.
// The pooled data behind an id as its text accessor sees it: the text at +0x10 and a count of the
// reads at +0x44.
// The pooled data behind a string id as its text accessor sees it (src/game/CGameManager.cpp).
// The pooled data behind a string id as its text accessor sees it.
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    base::global::CRntString str;
    u8 pad_0x40[0x4];
    // A union member: the original reloads the id's data pointer after the increment.
    union {
        s32 reads;
    };
};
