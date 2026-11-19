#pragma once

// A listener on a list of navigation-mesh listeners (the E.M.M.I.'s AI holds two, unit 811): the list it
// is on, and detaching takes it off (fn_7100566080). Names and layout are placeholders.
#include "types.h"

struct UnkMeshListeners;
struct UnkMeshListener;
void fn_7100566080(UnkMeshListeners* list, UnkMeshListener* listener);

struct UnkMeshListener {
    UnkMeshListeners* list;
    u8 pad_0x8[0x10];

    void Detach() {
        if (list) {
            fn_7100566080(list, this);
            list = nullptr;
        }
    }
};
