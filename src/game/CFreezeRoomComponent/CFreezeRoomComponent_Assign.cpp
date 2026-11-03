// CFreezeRoomComponent: the copy assignments of four reflected values of seven words (copied as one
// block), the same shape as fn_71002ee83c (base/UnkUnit424/UnkUnit424Assign.cpp). Names and layouts
// are placeholders.
#include "types.h"

namespace {
struct UnkFreezeRoomValue {
    u32 words[7];
};

__attribute__((always_inline)) inline UnkFreezeRoomValue* Assign(UnkFreezeRoomValue* dst,
                                                                 const UnkFreezeRoomValue* src) {
    if (dst != src)
        *dst = *src;
    return dst;
}
}  // namespace

void* fn_7100a24494(void* dst, const void* src) {
    return Assign(static_cast<UnkFreezeRoomValue*>(dst), static_cast<const UnkFreezeRoomValue*>(src));
}
void* fn_7100a244b8(void* dst, const void* src) {
    return Assign(static_cast<UnkFreezeRoomValue*>(dst), static_cast<const UnkFreezeRoomValue*>(src));
}
void* fn_7100a24548(void* dst, const void* src) {
    return Assign(static_cast<UnkFreezeRoomValue*>(dst), static_cast<const UnkFreezeRoomValue*>(src));
}
void* fn_7100a2456c(void* dst, const void* src) {
    return Assign(static_cast<UnkFreezeRoomValue*>(dst), static_cast<const UnkFreezeRoomValue*>(src));
}
