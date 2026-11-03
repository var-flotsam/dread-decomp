// CEmmyAttackComponent unit (tools/tu_map.py unit 812): two calls of fn_71008659c4 on the owner's collision
// object (+0x2a0) with the component's filter at +0x40, then the base step (fn_71011362e4). Names are
// placeholders.
#include "types.h"

struct UnkCollisionContainer;

struct UnkEmmyAttackOwner_b47 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkEmmyAttackComponent_b47 {
    u8 pad_0x0[0x30];
    UnkEmmyAttackOwner_b47* owner;  // 0x30
    u8 pad_0x38[0x8];
    u8 field_0x40[0x8];
};

void fn_71008659c4(UnkCollisionContainer* collision, u32 a, u32 b, void* filter);
void fn_71011362e4(UnkEmmyAttackComponent_b47* self);

void fn_710056a64c(UnkEmmyAttackComponent_b47* self) {
    UnkCollisionContainer* collision = self->owner->field_0x2a0;
    fn_71008659c4(collision, 0x400000, 0x80, self->field_0x40);
    fn_71008659c4(collision, 0x40000, 0x80000, self->field_0x40);
    fn_71011362e4(self);
}
