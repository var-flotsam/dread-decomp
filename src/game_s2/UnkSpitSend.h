#pragma once

// The request CBigkranXSpitAttack's functions send to the sub-object at +0x220 of their owner
// (fn_7101176690): a mode, two global descriptors and an all-bits mask whose statics are guarded.

#include "base/reflection/UnkReflStatics.h"
#include "types.h"

// A small flag value whose constructors are not constexpr, so its statics are guarded.
struct UnkFlags_SpitSend {
    u32 value;
    UnkFlags_SpitSend() { value = 0; }
};

struct UnkMask_SpitSend {
    u32 value;
    explicit UnkMask_SpitSend(const UnkFlags_SpitSend& f) { value = (u8)~f.value; }
};

inline const UnkFlags_SpitSend& UnkNone_SpitSend() {
    static UnkFlags_SpitSend s_value;
    return s_value;
}

inline const UnkMask_SpitSend& UnkAll_SpitSend() {
    static UnkMask_SpitSend s_value(UnkNone_SpitSend());
    return s_value;
}

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;  // reached through the GOT
extern UnkDesc_SpitSend g_unk_7172769740;  // reached through the GOT

// Sends a request with mode 0 under the two ids.
inline void UnkSpitSend(void* target, u64 hash, u64 kind) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}
