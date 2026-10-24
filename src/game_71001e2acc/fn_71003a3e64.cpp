// Constructor of a node (as in fn_71003771a8, src/game_s2/fn_71003771a8.cpp) over the base
// fn_71003944e8: sets the vtable, builds a runtime string from the empty string and names a string
// id, then finishes in vtable slot 0x18 of the node.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStrId;

typedef void** __attribute__((may_alias)) UnkVtable;

struct UnkNode_71003a3e64 {
    UnkVtable vtable;
    u8 pad_0x8[0x30];
    CRntString field_0x38;
    UnkStrId field_0x68;

    void vf_0x18() { reinterpret_cast<void (*)(UnkNode_71003a3e64*)>(vtable[3])(this); }
};

void fn_71003944e8(UnkNode_71003a3e64*);
extern void* g_unk_7101a28ac0[];

void fn_71003a3e64(UnkNode_71003a3e64* self) {
    fn_71003944e8(self);
    self->vtable = g_unk_7101a28ac0 + 2;
    new (&self->field_0x38) CRntString("");
    new (&self->field_0x68) UnkStrId("", true);
    self->vf_0x18();
}
