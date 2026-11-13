#pragma once

// The X parasite AI component's pick among its entries (fn_71006eb7cc, unit 1102 of tools/tu_map.py) and its
// result: a counted reference to the entry picked, released when the result goes out of scope. The component
// itself is laid out by the file that defines the search. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkXParasiteAI_b0252;

struct UnkXParasiteFound_b0252 : UnkRefObject {
    void* field_0x10;
};

struct UnkXParasiteResult_b0252 {
    UnkXParasiteFound_b0252* ptr;
    ~UnkXParasiteResult_b0252() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

UnkXParasiteResult_b0252 fn_71006eb7cc(UnkXParasiteAI_b0252* self);
