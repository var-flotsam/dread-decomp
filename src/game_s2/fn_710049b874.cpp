// Per-frame step of a component: runs the base step, and when the state at +0x14a08 is 1 builds a
// counted reference with fn_710023b580 on the member at +0x149e0, hands it to the one at +0x14a10
// (fn_7100234f78) and moves to state 2. Then, if the object that member points at has a target at
// +0x10, runs fn_710049b920 on the component.
#include "game_s2/UnkWeakHandle.h"

struct UnkTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkHolder {
    UnkTarget* field_0x0;
};

struct UnkComponent {
    u8 pad_0x0[0x149e0];
    u8 field_0x149e0[0x28];
    u32 field_0x14a08;
    u8 pad_0x14a0c[4];
    UnkHolder field_0x14a10;
};

void fn_71003aea40(UnkComponent* self);
UnkRefPtr fn_710023b580(void* member);
void fn_7100234f78(UnkHolder* holder, UnkRefPtr* reference);
void fn_710049b920(UnkComponent* self);

void fn_710049b874(UnkComponent* self) {
    fn_71003aea40(self);
    if (self->field_0x14a08 == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x149e0);
            fn_7100234f78(&self->field_0x14a10, &reference);
        }
        self->field_0x14a08 = 2;
    }
    if (self->field_0x14a10.field_0x0 && self->field_0x14a10.field_0x0->field_0x10)
        fn_710049b920(self);
}
