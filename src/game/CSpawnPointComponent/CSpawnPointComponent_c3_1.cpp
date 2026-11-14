// CSpawnPointComponent unit (tools/tu_map.py): equality of two vectors of links (reflection:
// CRntVector<CGameLink<CActor>>, the type of the component's tXCellTransformTargets and
// tDynamicSpawnPositions). Each link is resolved first (game/CGameLink.h); links are equal when their
// targets are. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/CBatalloonAIComponent.h"
#include "game/CGameLink.h"
#include "game/UnkTU1102.h"
#include "types.h"

struct CActor;

struct UnkRefObj_c3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object
    s32 refs;
};

struct UnkRef_c3 {
    UnkRefObj_c3* p;
    ~UnkRef_c3() {
        if (p && --p->refs == 0)
            p->vf_0x10();
    }
};

// Resolves `link` the way this function's copy does (fn_71003df480, fn_71006eb8cc) and returns its target.
static CActor* Get(CGameLink<CActor>& link) {
    if (link.state == 1) {
        {
            UnkRef_c3 ref = fn_71003df480(link.key);
            fn_71006eb8cc(&link.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.state = 2;
    }
    return link.Target();
}

bool fn_710113192c(base::UnkVector<CGameLink<CActor>>* self, base::UnkVector<CGameLink<CActor>>* other) {
    if (self->count != other->count)
        return false;
    for (u64 i = 0; i < self->count; ++i) {
        CGameLink<CActor>& a = self->data[i];
        CGameLink<CActor>& b = other->data[i];
        CActor* target = Get(a);
        if (Get(b) != target)
            return false;
    }
    return true;
}
