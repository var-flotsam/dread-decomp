// In the unit of COffset: the move constructors of two reflected keyed holders
// (src/game_5821e4/UnkKeyedHolder.h; fn_710061bc74 and fn_710061c188, slot 0x38 of their
// descriptors) whose record is an id/value element: the base vtable g_unk_7101a56178, a counted
// string id and 0x10 bytes. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "../../game_5821e4/UnkKeyedHolder.h"
#include "types.h"

using base::global::UnkStringData;

extern char g_unk_7101a56178[];  // the elements' base vtable

struct UnkOffsetIdValue {
    char* vtable;
    UnkStringData* id;
    u64 value[2];

    UnkOffsetIdValue(UnkOffsetIdValue&& other) {
        vtable = g_unk_7101a56178 + 0x10;
        id = other.id;
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
        __builtin_memcpy(value, other.value, sizeof(value));
    }
};

using UnkOffsetHolder = UnkKeyedHolder<u32, UnkOffsetIdValue>;
static_assert(sizeof(UnkOffsetHolder) == 0x30, "UnkOffsetHolder layout");

void fn_710061bc74(void* dst, const void* src) {
    new (dst) UnkOffsetHolder(std::move(*static_cast<UnkOffsetHolder*>(const_cast<void*>(src))));
}

void fn_710061c188(void* dst, const void* src) {
    new (dst) UnkOffsetHolder(std::move(*static_cast<UnkOffsetHolder*>(const_cast<void*>(src))));
}
