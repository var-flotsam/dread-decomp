// Copy assignment forwarding to a helper unless assigning to itself.
#include "types.h"

struct UnkStruct;
void fn_71005da04c(UnkStruct* self, const UnkStruct* other);

UnkStruct* fn_71005da01c(UnkStruct* self, const UnkStruct* other) {
    if (self != other)
        fn_71005da04c(self, other);
    return self;
}
