// CKraidAcidBlobsAttack: two identical assignments of a reflected value (fn_71005f1b8c and
// fn_71005f1bc8, the descriptor's slots 0x48 and 0x50): unless it is the source, the base part's 0x20
// bytes at +0x8, the word at +0x28 and the 0x1c bytes at +0x30 are copied. Names and layouts are
// placeholders.
#include "types.h"

namespace {
__attribute__((always_inline)) inline void* Assign(char* self, const char* other) {
    if (self != other) {
        __builtin_memcpy(self + 0x8, other + 0x8, 0x20);
        *reinterpret_cast<u64*>(self + 0x28) = *reinterpret_cast<const u64*>(other + 0x28);
        __builtin_memcpy(self + 0x30, other + 0x30, 0x1c);
    }
    return self;
}
}  // namespace

void* fn_71005f1b8c(void* dst, const void* src) {
    return Assign(static_cast<char*>(dst), static_cast<const char*>(src));
}

void* fn_71005f1bc8(void* dst, const void* src) {
    return Assign(static_cast<char*>(dst), static_cast<const char*>(src));
}
