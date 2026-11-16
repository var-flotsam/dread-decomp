#pragma once

// The lazily resolved holder that CSpawnPointComponent's unit (tools/tu_map.py, unit 2704) keeps in a
// base::global::CRntVector: 0x40 bytes with a virtual destructor, a callback vector at +0x8, a state at +0x30
// and a refcounted handle at +0x38, which is CGameLink<CActor>'s layout (include/game/CGameLink.h); its vtable
// is g_unk_7101cb2880. Its move is fn_710111fd80, its append fn_7101131b98, its insert fn_7101131ea8. Other
// views of it remain in src/game/UnkTU2704/UnkTU2704_SpawnLinks_ovb0171.cpp and
// src/game/CSpawnGroupComponent/CSpawnGroupComponent_Holders_ovb0065.cpp. Names and layout are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/UnkTU2704.h"
#include "types.h"

struct UnkSpawnHolder {
    virtual ~UnkSpawnHolder();
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30
    base::UnkRefHandle handle;       // 0x38

    // Makes this (just constructed) a copy of `other`.
    void CopyFrom(const UnkSpawnHolder& other) {
        state = 0;
        *reinterpret_cast<char**>(this) = reinterpret_cast<char*>(g_unk_7101cb2880) + 0x10;
        handle.ptr = nullptr;
        if (base::UnkRefCounted* p = other.handle.ptr) {
            ++p->ref_count;
            if (base::UnkRefCounted* old = handle.ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            handle.ptr = p;
        }
        state = other.state;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
};
static_assert(sizeof(UnkSpawnHolder) == 0x40, "layout");
