#pragma once

// A link to an object of the scene (reflection: CGameLink<CActor>, the type of every wp* member of the spawn
// point component; 0x40 bytes). It names its target by a key and resolves it once: while `state` is 1 the
// key is looked up into a counted reference to the target's weak handle, which is stored in `handle`, and the
// state becomes 2. The handle's object (+0x10) is the target. Several units resolve links with their own
// copies of the same two functions (the spawn point's unit with fn_710023b580 and fn_7100234f78).
// The link is polymorphic: reflection destroys one through its vtable (refl_Destroy, DestroyDestructorFirst)
// and so does an array of links (CRntVector<CGameLink<CActor>>::Free in fn_7101125c90); a member link is
// destroyed inline, the link's part (the handle) first, then its base's (the callback array). The member
// names and the base are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/reflection/ReflectedValue.h"
#include "types.h"

// The base of a link: the callback array the lookup reads (+0x8).
struct UnkGameLinkBase {  // vtable g_unk_7101a20998
    virtual ~UnkGameLinkBase() {}
    UnkOwnedErasedVector callbacks;  // 0x8: what the lookup reads
};

// The weak handle a resolved link holds (base::UnkRefCounted's layout): its target at +0x10.
template <class T>
struct CGameLinkHandle : base::UnkRefCounted {
    T* target;  // 0x10
};

// The vtable is CGameLink<CActor>'s; another instance has its own (UnkSpawnHolder's g_unk_7101cb2880 is
// likely CGameLink<CSpawnPointComponent>'s), which check.py would report as a RELOC where one is used.
template <class T>
struct CGameLink : UnkGameLinkBase {  // vtable g_unk_7101a20950
    u32 state;  // 0x30: 1 until resolved, then 2
    u8 pad_0x34[0x4];
    base::UnkRefHandle handle;  // 0x38

    virtual ~CGameLink() {
        if (base::UnkRefCounted* p = handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }

    // Reflection's value operations of each instance it describes (CGameLink<CActor> and others),
    // src/reflection/class_values_*.cpp.
    DREAD_REFLECTED_VALUE_MEMBERS;

    // The resolved target, or null; the link must have been resolved.
    T* Target() const {
        CGameLinkHandle<T>* h = static_cast<CGameLinkHandle<T>*>(handle.ptr);
        return h ? h->target : nullptr;
    }
};
