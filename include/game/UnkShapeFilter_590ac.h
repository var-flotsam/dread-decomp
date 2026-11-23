#pragma once

#include "game/UnkCollider.h"
#include "game/UnkTU2510.h"
#include "types.h"

// ---- fn_7100d590ac
struct UnkShapeFilter_590ac {
    void** vtable;
    UnkCollider* node;
    u64 field_0x10;
    void* filter;
    bool field_0x20;
    UnkShapeFilter_590ac(UnkCollider* n, void* f)
        : vtable(g_unk_7101aa8898 + 2), node(n), field_0x10(0), filter(f), field_0x20(false) {}
};
