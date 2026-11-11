// Copy of fn_7100d3b7d8 (src/game/logic/collision/CCollider/CCollider_TargetTyped_b46.cpp) in the CActor
// unit: the object a weak handle refers to, with its dynamic type descriptor refined twice from the CActor
// class descriptor, or the static descriptor alone when there is none. The class getter runs once per
// branch, through a second function the linker folded into it. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::UnkReflTypedPtr;

// The folded second getter; a static member mangles as folded::fn_7100227448, like a namespace.
struct folded {
    static const base::reflection::CClass* fn_7100227448();
};

UnkReflTypedPtr fn_7100234c64(UnkWeakHandle* const* handle) {
    UnkWeakHandle* h = *handle;
    void* obj = h ? h->target : nullptr;
    UnkReflTypedPtr r;
    if (obj) {
        fn_7100227448();
        char* t = fn_710009dcd8(g_unk_717269f450, obj);
        r.type = fn_710009dcd8(t, obj);
        r.ptr = obj;
    } else {
        folded::fn_7100227448();
        r.type = g_unk_717269f450;
        r.bits = 0;
    }
    return r;
}
