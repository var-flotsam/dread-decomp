// Constructor of a node (as in fn_7100374984, src/game_s2/fn_7100374984.cpp) whose name is a string:
// runs its base constructor (fn_71003944e8), sets the vtable, initialises its fields, then runs
// the post-construction virtual call.
#include <new>

#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct UnkNode_7100398128 {
    void** vtable;
    u8 pad_0x8[0x30];
    CRntString field_0x38;
    bool field_0x68;
    bool field_0x69;
    bool field_0x6a;
};

void fn_71003944e8(UnkNode_7100398128*);
extern void* g_unk_7101a27298[];

void fn_7100398128(UnkNode_7100398128* self) {
    fn_71003944e8(self);
    self->vtable = &g_unk_7101a27298[2];
    new (&self->field_0x38) CRntString("");
    self->field_0x68 = true;
    self->field_0x69 = false;
    self->field_0x6a = false;
    reinterpret_cast<void (*)(UnkNode_7100398128*)>(self->vtable[3])(self);
}
