// Constructor of a node: runs its base constructor, sets the vtable, names one string id and leaves
// a second as the empty string.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrId;
using base::global::UnkStrIdBase;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStrId field_0x30;
    UnkStrIdBase field_0x38;
};

void fn_7100393e68(UnkNode*);
extern void* g_unk_7101a254e0[];

void fn_7100375dac(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = &g_unk_7101a254e0[2];
    new (&self->field_0x30) UnkStrId("", true);
    self->field_0x38.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x38.data = &g_unk_7101d47108->empty;
}
