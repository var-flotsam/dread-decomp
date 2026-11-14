#pragma once

// A link to an object of the scene (reflection: CGameLink<CActor>, the type of every wp* member of the spawn
// point component; the members are 0x40 bytes apart where nothing else sits between them). It names its
// target by a key and resolves it once: while `state` is 1 the key is looked up into a counted reference to the
// target's weak handle, which is stored in `handle`, and the state becomes 2. The handle's object (+0x10) is
// the target. Several units resolve links with their own copies of the same two functions (the spawn point's
// unit with fn_710023b580 and fn_7100234f78). The member names and the layout are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "types.h"

// The weak handle a resolved link holds (base::UnkRefCounted's layout): its target at +0x10.
template <class T>
struct CGameLinkHandle : base::UnkRefCounted {
    T* target;  // 0x10
};

template <class T>
struct CGameLink {
    u8 pad_0x0[0x8];
    u8 key[0x28];  // 0x8: what the lookup reads
    u32 state;     // 0x30: 1 until resolved, then 2
    u8 pad_0x34[0x4];
    base::UnkRefHandle handle;  // 0x38

    // The resolved target, or null; the link must have been resolved.
    T* Target() const {
        CGameLinkHandle<T>* h = static_cast<CGameLinkHandle<T>*>(handle.ptr);
        return h ? h->target : nullptr;
    }
};
