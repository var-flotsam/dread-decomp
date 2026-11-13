#pragma once

// The callback set of vtable 0x7101a20950 and move constructor fn_71002458a4 (0x40 bytes): the
// shared base and the refcounted owner. The address is in the name because other set classes share
// the layout under other vtables (game/UnkCallbackBase.h); one placeholder name for both would give
// their constructors and vtables two addresses (tools/port_addresses.py). Names are placeholders.

#include "base/UnkErasedVector.h"
#include "game/UnkCallbackBase.h"

struct UnkCallbackSet_7101a20950 : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkCallbackSet_7101a20950() : owner(nullptr) {}
    UnkCallbackSet_7101a20950(const UnkCallbackSet_7101a20950& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }

    UnkCallbackSet_7101a20950(UnkCallbackSet_7101a20950&& other);  // fn_71002458a4

    __attribute__((always_inline)) ~UnkCallbackSet_7101a20950() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};
