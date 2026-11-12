// The "Version" attribute of a registered file type, split into (major, minor) halfwords
// (called by fn_710000ccd8 in src/base/animation/CBlendSpaceResource.cpp). Names are placeholders.
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkVersionEntry;
// The attribute stored under `key`, as a reflected value (empty when absent).

bool fn_71000e6ce0(UnkVersionEntry* self, u16* major, u16* minor) {
    UnkReflValue v = fn_710009ddd4(self, fn_7100005070());
    bool ok = false;
    const base::reflection::CType* type = reinterpret_cast<const base::reflection::CType*>(v.type);
    if (type && fn_710009d50c(type, fn_7100005178())) {
        u32 version = *base::reflection::refl_ArgValue<u32>(&v);
        if (major)
            *major = version >> 16;
        if (minor)
            *minor = version;
        ok = true;
    }
    fn_71000a1c60(&v);
    return ok;
}
