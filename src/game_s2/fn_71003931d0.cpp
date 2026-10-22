// Constructor of a node: runs its base constructor, sets the vtable and a float, names three string
// ids with the empty string and clears 11 bytes at +0x48.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x24];
    f32 field_0x2c;
    UnkStrId field_0x30;
    UnkStrId field_0x38;
    UnkStrId field_0x40;
    u8 field_0x48[0xb];
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a26cb0[];

void fn_71003931d0(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a26cb0 + 2;
    self->field_0x2c = 1.0f;
    new (&self->field_0x30) UnkStrId("", true);
    new (&self->field_0x38) UnkStrId("", true);
    new (&self->field_0x40) UnkStrId("", true);
    __builtin_memset(self->field_0x48, 0, 11);
}
