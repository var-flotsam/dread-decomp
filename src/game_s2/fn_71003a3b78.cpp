// Destructor: resets the vtable, releases the string id at +0x68, then the sub-object at +0x38 and the base.
#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

extern void* g_unk_7101a28ac0[];

struct UnkObj {
    void** vtable;
    u8 pad_0x8[0x60];
    UnkStrIdTwoStep field_0x68;
};

void fn_710007ecd4(void*);
void fn_7100394530(void*);

void fn_71003a3b78(UnkObj* self) {
    self->vtable = &g_unk_7101a28ac0[2];
    self->field_0x68.~UnkStrIdTwoStep();
    fn_710007ecd4(reinterpret_cast<char*>(self) + 0x38);
    fn_7100394530(self);
}
