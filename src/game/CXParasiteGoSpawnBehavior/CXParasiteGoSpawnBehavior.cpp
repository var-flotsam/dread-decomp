// Two identical copy assignments in the CXParasiteGoSpawnBehavior unit: the three words at +0x8
// are copied as a block and the member at +0x20 is assigned by fn_7100246d70. Names and layouts are
// placeholders.
#include "types.h"

void fn_7100246d70(void* self, const void* other);

struct UnkGoSpawnParams {
    void** vtable;
    struct {
        u64 words[3];
    } field_0x8;
    u8 field_0x20[0x8];

    __attribute__((always_inline)) UnkGoSpawnParams& operator=(const UnkGoSpawnParams& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            fn_7100246d70(field_0x20, other.field_0x20);
        }
        return *this;
    }
};

UnkGoSpawnParams* fn_71007cccd8(UnkGoSpawnParams* self, const UnkGoSpawnParams* other) { return &(*self = *other); }
UnkGoSpawnParams* fn_71007ccd1c(UnkGoSpawnParams* self, const UnkGoSpawnParams* other) { return &(*self = *other); }
