#pragma once

#include "game/UnkArray.h"
#include "game/UnkLinked.h"
#include "types.h"

struct UnkLinkGroup {
    UnkArray<UnkLinked*> linked;
    UnkArray<u64> field_0x28;
    UnkArray<u64> field_0x50;
    bool field_0x78;

    __attribute__((always_inline)) UnkLinkGroup() : field_0x78(false) {}
    ~UnkLinkGroup() {
        for (UnkLinked* p : linked)
            p->owner = nullptr;
        linked.count = 0;
        field_0x28.count = 0;
        field_0x50.count = 0;
    }
};
