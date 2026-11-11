// GUI::CDisplayObjectContainer unit (tools/tu_map.py unit 2748): copy of fn_7100d3b7d8
// (src/game/logic/collision/CCollider/CCollider_TargetTyped_b46.cpp) for this class. The object a weak
// handle refers to, paired with its dynamic type descriptor (the class's static descriptor refined
// twice), or with the static descriptor alone when there is none. The class getter runs once per
// branch, through a second function the linker folded into it. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::UnkReflTypedPtr;

// The folded second getter; a static member mangles as folded::fn_71011d7b84, like a namespace.
struct folded {
    static const base::reflection::CClass* fn_71011d7b84();
};

UnkReflTypedPtr fn_71011da640(UnkWeakHandle* const* handle) {
    UnkWeakHandle* h = *handle;
    void* obj = h ? h->target : nullptr;
    UnkReflTypedPtr r;
    if (obj) {
        fn_71011d7b84();
        char* t = fn_710009dcd8(g_unk_7172a0f120, obj);
        r.type = fn_710009dcd8(t, obj);
        r.ptr = obj;
    } else {
        folded::fn_71011d7b84();
        r.type = g_unk_7172a0f120;
        r.bits = 0;
    }
    return r;
}
