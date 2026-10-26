// Builds a blackboard property definition's qualified name as a string (the counterpart of
// fn_7100ae9a10, which interns it as an id).
#include "base/global/CRntString.h"
#include "game/UnkGameBlackboard.h"

using base::global::CStrId;

// The string data's use count, a union member like ref_count (no type-based alias info, so the
// id is reloaded after the increment; see fn_7100ae9a10).
struct UnkStringUse {
    u8 unk_0x0[0x44];
    union {
        u32 use_count;
    };
};

// The id's string (a CRntString at +0x10 of the string data), with its use count raised.
static inline const base::global::CRntString* UseStr(const CStrId& id) {
    ++reinterpret_cast<UnkStringUse*>(id.data)->use_count;
    return reinterpret_cast<const base::global::CRntString*>(reinterpret_cast<char*>(id.data) + 0x10);
}

static inline bool HasValue(const CStrId& id) {
    return (u64)id.data != (u64)g_unk_7101d47108 + 0x10;
}

void fn_7100ae9d7c(UnkQualifiedName* self, UnkIdSource* source, const CStrId& prefix) {
    auto* str = reinterpret_cast<base::global::CRntString*>(self);
    self->source = source;
    if (HasValue(prefix)) {
        fn_710007f3b4(str, UseStr(prefix));
        fn_710007f5ac(str, ':');
    }
    if (HasValue(source->vf_0x20())) {
        fn_710007f3b4(str, UseStr(source->vf_0x20()));
        fn_710007f5ac(str, ':');
    }
    fn_710007f3b4(str, UseStr(source->vf_0x28()));
}
