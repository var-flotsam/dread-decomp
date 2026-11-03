// Two identical copy assignments of a reflected 0x40-byte value type in unit 233 (pattern of
// fn_71007ccd1c): three words at +0x0 are copied as a block, the member at +0x18 is assigned by
// fn_71001abd04. Names and layouts are placeholders.
#include "types.h"

void fn_71001abd04(void* self, const void* other);

namespace {
struct UnkUnit233Value {
    struct {
        u64 words[3];
    } field_0x0;
    u8 field_0x18[0x28];

    __attribute__((always_inline)) UnkUnit233Value& operator=(const UnkUnit233Value& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            fn_71001abd04(field_0x18, other.field_0x18);
        }
        return *this;
    }
};
}  // namespace

void* fn_71001abc7c(void* dst, const void* src) {
    return &(*static_cast<UnkUnit233Value*>(dst) = *static_cast<const UnkUnit233Value*>(src));
}
void* fn_71001abcc0(void* dst, const void* src) {
    return &(*static_cast<UnkUnit233Value*>(dst) = *static_cast<const UnkUnit233Value*>(src));
}
