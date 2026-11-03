// Unit 2506: the invoker of a std::function holding a bound member-function pointer called on an
// object with a flag and a float. Names and layouts are placeholders.
#include "types.h"

class UnkUnit2506Target;
typedef void (UnkUnit2506Target::*UnkUnit2506Method)(bool, f32);

struct UnkUnit2506Bound {
    void* vtable;
    UnkUnit2506Method method;
};

void fn_7100d4b8c8(UnkUnit2506Bound* self, UnkUnit2506Target** target, const f32* value, const u8* flag) {
    f32 v = *value;
    UnkUnit2506Method method = self->method;
    UnkUnit2506Target* obj = *target;
    u8 f = *flag;
    (obj->*method)(f != 0, v);
}
