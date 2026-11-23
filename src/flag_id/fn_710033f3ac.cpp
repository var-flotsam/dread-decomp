// Calls virtual slot 0xa0 with 1, then a follow-up helper.
#include "types.h"

struct UnkVObj_33f3ac {
    void** vtable;
};

void fn_71002f9264(UnkVObj_33f3ac* self);

void fn_710033f3ac(UnkVObj_33f3ac* self) {
    using Fn = void (*)(UnkVObj_33f3ac*, s32);
    reinterpret_cast<Fn>(self->vtable[0xa0 / 8])(self, 1);
    fn_71002f9264(self);
}
