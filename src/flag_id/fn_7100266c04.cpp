// Copy assignment: copies two fields, then a helper finishes the copy.
#include "types.h"

struct UnkStruct_0x10 {
    u32 field_0x0;
    u64 field_0x8;
};

void fn_7100266c44(UnkStruct_0x10* self, const UnkStruct_0x10* other);

UnkStruct_0x10* fn_7100266c04(UnkStruct_0x10* self, const UnkStruct_0x10* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->field_0x8 = other->field_0x8;
        fn_7100266c44(self, other);
    }
    return self;
}
