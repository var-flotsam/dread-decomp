// build-flags: no-strict-aliasing
// CAbilityShinespark's unit (433): stores a reflected value under a string id in a section's
// property arrays: an existing entry takes the value through its type's assignment; otherwise the
// id and a copy of the value are appended. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct UnkValueMap_7100332c80;

static inline UnkReflValue* Values(UnkPropertyArrays& props) {
    return static_cast<UnkReflValue*>(props.values);
}

static inline UnkReflValue* Find(UnkPropertyArrays& props, const CStrId& key) {
    for (u64 i = 0; i < props.count; ++i)
        if (props.ids[i].data == key.data)
            return &Values(props)[i];
    return &Values(props)[props.count];
}

// Adds `key` and a copy of `value` at the end, growing both arrays by the step when full.
inline void Append(UnkPropertyArrays& props, const CStrId& key, const UnkReflValue* value) {
    if (props.count == props.capacity)
        fn_7100093dd0(&props, props.capacity + props.step);
    new (&props.ids[props.count]) CStrId(key);
    fn_71000a179c(&Values(props)[props.count], value);
    ++props.count;
}

void fn_7100332dc4(UnkValueMap_7100332c80* map, const u64* key_, const UnkTypedRef* value) {
    UnkSectionData* self = reinterpret_cast<UnkSectionData*>(map);
    const CStrId& key = *reinterpret_cast<const CStrId*>(key_);
    UnkPropertyArrays& props = self->properties;

    UnkReflValue* it = Find(props, key);
    if (it != &Values(props)[props.count]) {
        UnkTypedRef dst;
        dst.type = reinterpret_cast<const CType*>(it->type);
        if (const char* type = it->type)
            dst.ptr = it->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? it->field_0x0 : it;
        else
            dst.ptr = nullptr;
        fn_710009d7a8(dst.type, &dst, const_cast<UnkTypedRef*>(value));
        return;
    }

    UnkReflValue copy;
    fn_71000a1978(&copy, value, 0, self->flags);
    Append(props, key, &copy);
    fn_71000a1c60(&copy);
}
