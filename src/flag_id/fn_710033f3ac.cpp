// Calls virtual slot 0xa0 with 1, then a follow-up helper.
#include "types.h"

struct UnkVObj {
    void** vtable;
};

void fn_71002f9264(UnkVObj* self);

void fn_710033f3ac(UnkVObj* self) {
    using Fn = void (*)(UnkVObj*, s32);
    reinterpret_cast<Fn>(self->vtable[0xa0 / 8])(self, 1);
    fn_71002f9264(self);
}
