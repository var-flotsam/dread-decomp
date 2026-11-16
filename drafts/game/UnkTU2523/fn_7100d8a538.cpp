// UnkTU2523 (tools/tu_map.py unit 2523): the conversion that makes a link (a callback set, vtable
// g_unk_7101a33fa0, holding a counted reference to a weak handle) refer to the target of a weak handle:
// it builds a resolved link to the target and moves it in. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLinkBase_ld8a538 {
    void** vtable;
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30: 2 once resolved

    UnkLinkBase_ld8a538() : state(0) {}
    ~UnkLinkBase_ld8a538() { vtable = &g_unk_7101a20998[2]; }
};

struct UnkLink_ld8a538 : UnkLinkBase_ld8a538 {
    UnkWeakHandle* handle;  // 0x38

    UnkLink_ld8a538() {
        vtable = &g_unk_7101a33fa0[2];
        handle = nullptr;
    }
    void SetHandle(UnkWeakHandle* p) {
        if (handle != p) {
            if (p)
                ++p->ref_count;
            if (UnkWeakHandle* old = handle) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            handle = p;
        }
    }
    ~UnkLink_ld8a538() {
        vtable = &g_unk_7101a33fa0[2];
        if (UnkWeakHandle* p = handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    UnkLink_ld8a538& operator=(UnkLink_ld8a538&& other) {
        UnkErasedVector* from = &other.callbacks;
        fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&handle),
                      reinterpret_cast<const base::UnkRefHandle*>(&other.handle));
        state = other.state;
        fn_710023b29c(&callbacks, from);
        other.state = 0;
        fn_7100ad3588(from);
        ResetRef(&other.handle);
        return *this;
    }
};
static_assert(sizeof(UnkLink_ld8a538) == 0x40, "layout");

UnkLink_ld8a538* fn_7100d8a538(UnkLink_ld8a538* self, UnkWeakHandle* const* other) {
    if (other != static_cast<const void*>(self)) {
        // A link resolved to the handle's target.
        UnkLink_ld8a538 link;
        link.SetHandle(GetWeakHandle((*other)->target));
        link.state = 2;
        *self = static_cast<UnkLink_ld8a538&&>(link);
    }
    return self;
}
