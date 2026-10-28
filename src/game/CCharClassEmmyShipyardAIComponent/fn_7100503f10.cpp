// Constructor of a node: runs its base constructor, sets the vtable, initialises its fields and
// names its string ids, then runs the post-construction virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x5f8];
    u16 field_0x600;
    u8 field_0x602;
    u8 pad_0x603[0x5];
    UnkStrId field_0x608;
};

void fn_710051eaa0(UnkNode*);
extern void* g_unk_7101a46dc8[];

void fn_7100503f10(UnkNode* self) {
    fn_710051eaa0(self);
    self->vtable = &g_unk_7101a46dc8[2];
    self->field_0x600 = 256;
    self->field_0x602 = 1;
    self->field_0x608 = UnkStrId("CurrentScenario.OnEmmyShipyardAbilityObtained", true);
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
