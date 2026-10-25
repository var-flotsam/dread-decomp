// Destructor: resets the vtable, releases the string id at +0x60, then the sub-object at +0x30 and the base.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

extern void* g_unk_7101a25010[];

struct UnkObj {
    void** vtable;
    u8 pad_0x8[0x58];
    UnkStrIdTwoStep field_0x60;
};

void fn_7100393ea0(void*);

void fn_7100372ba0(UnkObj* self) {
    self->vtable = &g_unk_7101a25010[2];
    self->field_0x60.~UnkStrIdTwoStep();
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(reinterpret_cast<char*>(self) + 0x30));
    fn_7100393ea0(self);
}
