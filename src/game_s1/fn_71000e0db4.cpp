// Copies of fn_7100c8f544 (src/game_aca57c/fn_7100ad2728.cpp) for other record sizes, naming their own callees and data
// (the comments are adapted from the original's): fn_71000e0db4, fn_7100273ce4. A value and a pointer bound to a pair
// that refers to them (state 2) while the pointer is set. Each function builds one from the pointer held by its first
// argument and assigns it to the second: the value and the pointer are copied and the target's pair is rebound to the
// target's own members (or reset). The in-place second value is destroyed out of line. Two record sizes.
#include "base/global/CRntString.h"
#include "game/UnkBoundPair.h"
#include "game/UnkBoundRef.h"
#include "game/UnkTU1629.h"
#include "game/navmesh/CFixedDynamicSmartLinkRule.h"
#include "types.h"

template <class Y>
__attribute__((always_inline)) static inline void AssignBound(Y* const* src, UnkBoundRef<Y>* out) {
    *out = UnkBoundRef<Y>(*src);
}

template <u32 Size, void (*Destroy)(void*)>
struct UnkDestroyed {
    u64 unk_0x0[Size / 8];

    __attribute__((always_inline)) ~UnkDestroyed() { Destroy(this); }
};

// The 0x30-byte values are strings (base/global/CRntString.h).
inline void DestroyString(void* self) { fn_710007ecd4(static_cast<base::global::CRntString*>(self)); }

using UnkBound30 = UnkDestroyed<0x30, DestroyString>;
using UnkBound50 = UnkDestroyed<0x50, fn_710026ead0>;

void fn_71000e0db4(UnkBound30* const* src, UnkBoundRef<UnkBound30>* out) { AssignBound(src, out); }
void fn_7100273ce4(UnkBound50* const* src, UnkBoundRef<UnkBound50>* out) { AssignBound(src, out); }
