#pragma once

// The handle the GUI registry (the root g_unk_717271e8f8) hands out for a named composition: a
// reference-counted weak handle (count at +8, the looked-up display object at +0x10) released
// through vf_0x10. fn_71011b69d8 (src/game_hi/fn_71011b69d8.cpp) looks a name up and loads it on
// first use. Names and layouts are placeholders.

#include "base/global/CRntString.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkGuiRoot;

struct UnkGuiHandleObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    UnkGuiDisplayObject* target;
};

struct UnkGuiHandle {
    UnkGuiHandleObj* ptr;

    UnkGuiDisplayObject* Get() const { return ptr ? ptr->target : nullptr; }
    ~UnkGuiHandle() {
        if (UnkGuiHandleObj* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

UnkGuiHandle fn_71011b69d8(UnkGuiRoot* registry, const base::global::CRntString& name);
