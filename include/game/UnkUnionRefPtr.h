#pragma once

#include "game/UnkUnionRefObject.h"

struct UnkUnionRefPtr {
    UnkUnionRefObject* ptr;

    ~UnkUnionRefPtr() {
        if (UnkUnionRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
