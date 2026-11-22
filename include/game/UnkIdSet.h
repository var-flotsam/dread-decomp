#pragma once

#include "game/UnkIdList.h"
#include "types.h"

struct UnkIdSet;

struct UnkIdSet {
    UnkIdList field_0x0;
    UnkIdList field_0x28;
    UnkIdList field_0x50;
    UnkIdList field_0x78;
    UnkIdList field_0xa0;
    UnkIdList field_0xc8;

    bool IsEmpty() const {
        return field_0x28.count == 0 && field_0x0.count == 0 && field_0x50.count == 0 &&
               field_0x78.count == 0 && field_0xc8.count == 0 && field_0xa0.count == 0;
    }
};
