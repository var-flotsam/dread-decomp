// base::core::CBaseObject unit: a one-character code for a reflected type (b bool, i int, f float,
// v vector, e enum, s string, ...), 'a' for an asset id and 'n' for anything else.
// Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/core/CBaseObject.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU25.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

s32 fn_7100024140(const CType* type) {
    if (type == fn_710000d228())
        return 'b';
    if (type == fn_7100014768())
        return 'i';
    if (type == fn_7100005178())
        return 'u';
    if (type == fn_7100024324())
        return 'I';
    if (type == fn_7100010e58())
        return 'U';
    if (type == fn_710000abbc())
        return 'f';
    if (type == fn_7100024504())
        return 'y';
    if (type == fn_71000246e4())
        return 'v';
    if (type == fn_71000248c4())
        return 'w';
    if (type == fn_710000ffd0())
        return 'c';

    const UnkReflObject* object = reinterpret_cast<const UnkReflObject*>(type);
    if (fn_710009d760(object, fn_710009921c()))
        return 'e';
    if (fn_710009d760(object, fn_710009a11c()))
        return 'F';
    if (fn_710009d52c(type, fn_7100007f84()))
        return 's';
    if (fn_710009d52c(type, fn_710000ad9c()))
        return 'p';
    return fn_710009d52c(type, &g_unk_7101d4dad8) ? 'a' : 'n';
}
