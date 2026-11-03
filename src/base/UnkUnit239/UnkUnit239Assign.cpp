// Two identical copy assignments of a reflected 0x40-byte value type in unit 239 of
// tools/tu_map.py: two words at +0x0 and a 0x1c-byte block at +0x10 copied as blocks, a string id
// at +0x30 and a word at +0x38. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

namespace {
struct UnkUnit239Value {
    struct {
        u64 words[2];
    } field_0x0;
    struct {
        u32 words[7];
    } field_0x10;
    base::global::CStrId field_0x30;
    u64 field_0x38;

    __attribute__((always_inline)) UnkUnit239Value& operator=(const UnkUnit239Value& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            field_0x10 = other.field_0x10;
            field_0x30 = other.field_0x30;
            field_0x38 = other.field_0x38;
        }
        return *this;
    }
};
}  // namespace

void* fn_71001b3af0(void* dst, const void* src) {
    return &(*static_cast<UnkUnit239Value*>(dst) = *static_cast<const UnkUnit239Value*>(src));
}
void* fn_71001b3bac(void* dst, const void* src) {
    return &(*static_cast<UnkUnit239Value*>(dst) = *static_cast<const UnkUnit239Value*>(src));
}
