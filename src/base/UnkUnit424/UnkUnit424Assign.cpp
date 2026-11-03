// From unit 424 of tools/tu_map.py: the two copy assignments of the reflected 0x20-byte value
// described by fn_71002ea654 (seven words, copied as one block). Names and layouts are
// placeholders.
#include "types.h"

namespace {
struct UnkUnit424Value {
    u32 words[7];
};

__attribute__((always_inline)) inline UnkUnit424Value* Assign(UnkUnit424Value* dst, const UnkUnit424Value* src) {
    if (dst != src)
        *dst = *src;
    return dst;
}
}  // namespace

void* fn_71002ee818(void* dst, const void* src) {
    return Assign(static_cast<UnkUnit424Value*>(dst), static_cast<const UnkUnit424Value*>(src));
}
void* fn_71002ee83c(void* dst, const void* src) {
    return Assign(static_cast<UnkUnit424Value*>(dst), static_cast<const UnkUnit424Value*>(src));
}
