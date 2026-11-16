// CInputComponent's unit: files a list of flag values under a key (fn_71008724f8). When there is
// something to do it may reset the component (fn_7100872268) and clear its pressed state, then hands a
// copy of the list to the block at +0xad0 (fn_710086fca0). Names and layouts are placeholders.
#include "../../game_s7/UnkFlagListS7.h"
#include "base/global/CRntVector.h"
#include "game/CInputComponent.h"
#include "types.h"

using unk_flag_list::UnkFlagList;
using unk_flag_list::UnkFlagListKey;

struct UnkInputBlock_l8724f8;
void fn_710086fca0(UnkInputBlock_l8724f8* self, UnkFlagListKey* key, UnkFlagList values, bool b, bool a);

struct UnkInputTarget_l8724f8 {
    u8 pad_0x0[0x3c];
    u32 field_0x3c;
};

struct UnkInputTargetNode_l8724f8 {
    UnkInputTarget_l8724f8* target;
    UnkInputTargetNode_l8724f8* next;
};

// Five pressed-state flags.
struct UnkInputPressed_l8724f8 {
    bool field_0x0;
    bool field_0x1;
    bool field_0x2;
    bool field_0x3;
    bool field_0x4;

    bool Any() const { return field_0x0 || field_0x4 || field_0x3 || field_0x1 || field_0x2; }
};

struct UnkInputComp_l8724f8 {
    u8 pad_0x0[0x9e0];
    u64 field_0x9e0;
    u32 field_0x9e8;
    u8 pad_0x9ec[0xab0 - 0x9ec];
    UnkInputTargetNode_l8724f8* targets;  // 0xab0
    u8 pad_0xab8[0xad0 - 0xab8];
    u8 block[0xb04 - 0xad0];  // 0xad0
    UnkInputPressed_l8724f8 pressed;  // 0xb04
};
static_assert(__builtin_offsetof(UnkInputComp_l8724f8, pressed) == 0xb04, "UnkInputComp_l8724f8 layout");

void fn_71008724f8(void* target, UnkFlagListKey* key, UnkFlagList* values, bool a, bool b, bool c, bool d) {
    UnkInputComp_l8724f8* self = static_cast<UnkInputComp_l8724f8*>(target);
    if (values->count != 0 || a || b) {
        if (c)
            fn_7100872268(reinterpret_cast<UnkInputComponent*>(self));
        if (d) {
            self->field_0x9e8 = 0;
            self->field_0x9e0 = 0;
            for (UnkInputTargetNode_l8724f8* n = self->targets; n; n = n->next)
                n->target->field_0x3c = 0;
            if (self->pressed.Any())
                self->pressed = UnkInputPressed_l8724f8();
        }
    }
    fn_710086fca0(reinterpret_cast<UnkInputBlock_l8724f8*>(self->block), key, *values, b, a);
}
