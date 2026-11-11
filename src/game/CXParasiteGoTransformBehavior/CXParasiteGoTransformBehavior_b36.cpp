// Two identical copy assignments in the CXParasiteGoTransformBehavior unit (as those of
// CXParasiteGoSpawnBehavior.cpp): the three words at +0x8 are copied as a block, then the handle at
// +0x58, the word at +0x50, the array at +0x28 and the member at +0x60 are assigned. Names and
// layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkGoTransformParams {
    void** vtable;
    struct {
        u64 words[3];
    } field_0x8;
    u8 pad_0x20[0x8];
    UnkErasedVector field_0x28;
    u32 field_0x50;
    base::UnkRefHandle field_0x58;
    u8 field_0x60[0x8];

    __attribute__((always_inline)) UnkGoTransformParams& operator=(const UnkGoTransformParams& other) {
        if (this != &other) {
            const base::UnkRefHandle* handle = &other.field_0x58;
            field_0x8 = other.field_0x8;
            fn_7100234f78(&field_0x58, handle);
            field_0x50 = other.field_0x50;
            fn_710023b29c(&field_0x28, &other.field_0x28);
            fn_7100246d70(field_0x60, other.field_0x60);
        }
        return *this;
    }
};
static_assert(__builtin_offsetof(UnkGoTransformParams, field_0x60) == 0x60, "placeholder layout");

UnkGoTransformParams* fn_71007cd790(UnkGoTransformParams* self, const UnkGoTransformParams* other) {
    return &(*self = *other);
}
UnkGoTransformParams* fn_71007cd7f8(UnkGoTransformParams* self, const UnkGoTransformParams* other) {
    return &(*self = *other);
}
