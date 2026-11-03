// CLogicCamera unit (tools/tu_map.py unit 2655): copy of fn_7100d3b7d8
// (src/game/logic/collision/CCollider/CCollider_TargetTyped_b46.cpp) for this class. The object a weak
// handle refers to, paired with its dynamic type descriptor (the class's static descriptor refined
// twice), or with the static descriptor alone when there is none. The class getter runs once per
// branch, through a second function the linker folded into it. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::UnkReflTypedPtr;

// The folded second getter; a static member mangles as folded::fn_7101050878, like a namespace.
struct folded {
    static const base::reflection::CClass* fn_7101050878();
};
extern char g_unk_71729fe020[] __attribute__((visibility("hidden")));

UnkReflTypedPtr fn_7101052000(UnkWeakHandle* const* handle) {
    UnkWeakHandle* h = *handle;
    void* obj = h ? h->target : nullptr;
    UnkReflTypedPtr r;
    if (obj) {
        fn_7101050878();
        char* t = fn_710009dcd8(g_unk_71729fe020, obj);
        r.type = fn_710009dcd8(t, obj);
        r.ptr = obj;
    } else {
        folded::fn_7101050878();
        r.type = g_unk_71729fe020;
        r.bits = 0;
    }
    return r;
}
