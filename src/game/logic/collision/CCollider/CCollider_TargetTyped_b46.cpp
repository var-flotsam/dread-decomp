// game::logic::collision::CCollider unit (tools/tu_map.py unit 2503): the object a weak handle
// refers to (include/game_s2/UnkWeakHandle.h), paired with its dynamic type descriptor (the
// collider class's static descriptor refined twice, as refl_PointeeTyped in
// include/base/reflection/UnkReflTypedPtr.h), or with the static descriptor alone when there is
// none. The class getter runs once per branch, through a second function the linker folded into
// it. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::UnkReflTypedPtr;

// The folded second getter; a static member mangles as folded::fn_7100d361c0, like a namespace.
struct folded {
    static const base::reflection::CClass* fn_7100d361c0();
};
extern char g_unk_71727b29a8[] __attribute__((visibility("hidden")));

UnkReflTypedPtr fn_7100d3b7d8(UnkWeakHandle* const* handle) {
    UnkWeakHandle* h = *handle;
    void* obj = h ? h->target : nullptr;
    UnkReflTypedPtr r;
    if (obj) {
        fn_7100d361c0();
        char* t = fn_710009dcd8(g_unk_71727b29a8, obj);
        r.type = fn_710009dcd8(t, obj);
        r.ptr = obj;
    } else {
        folded::fn_7100d361c0();
        r.type = g_unk_71727b29a8;
        r.bits = 0;
    }
    return r;
}
