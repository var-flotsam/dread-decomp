// Constructor: runs the base constructor, sets the vtable, builds an empty runtime string at 0x30,
// makes the string id at 0x60 the empty string (creating the pool first when there is none) and
// sets the 16-bit value at 0x68 to 1.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;

struct UnkNode {
    void** vtable;
    u8 pad_0x8[0x28];
    CRntString field_0x30;
    UnkStringData* field_0x60;
    u16 field_0x68;
};

void fn_7100393e68(UnkNode* self);
extern void* g_unk_7101a25010[];

void fn_7100372788(UnkNode* self) {
    fn_7100393e68(self);
    self->vtable = g_unk_7101a25010 + 2;
    new (&self->field_0x30) CRntString("");
    self->field_0x60 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x60 = &g_unk_7101d47108->empty;
    self->field_0x68 = 1;
}
