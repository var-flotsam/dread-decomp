// CCutsceneComponent's unit (tools/tu_map.py): fn_71007ada28 is the reflected invoker of the component's
// "is loaded" query (src/game/CCutsceneComponent/CCutsceneComponent_IsLoaded_b53.cpp, inlined): it asks the
// cutscene manager (g_unk_7101d47330, fn_71011986e0) about the path id at +0x150 of the object the argument
// holds at +8 and returns the answer as a reflected bool. Names are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU0.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::reflection::UnkReflValue;

// As src/game/CCutsceneComponent/CCutsceneComponent_IsLoaded_b53.cpp declares them.
u32 fn_71011986e0(void* manager, const CFilePathStrId* path);

struct UnkCutsceneComponent_A3_b72 {
    u8 pad_0x0[0x150];
    CFilePathStrId path;  // 0x150
};

static inline __attribute__((always_inline)) bool IsLoaded_A3_b72(UnkCutsceneComponent_A3_b72* self) {
    void* manager = g_unk_7101d47330;
    CFilePathStrId path(self->path);
    return fn_71011986e0(manager, &path) != 0;
}

UnkReflValue fn_71007ada28(void*, char* obj) {
    bool value = IsLoaded_A3_b72(*reinterpret_cast<UnkCutsceneComponent_A3_b72**>(obj + 8));
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
