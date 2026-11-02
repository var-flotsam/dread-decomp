// A constructor in CCharClassFXLinkEvent's unit: runs the base constructor (fn_7100067ea4), sets
// the vtable, builds the string id at +0x10 from "" and makes the ids at +0x20 and +0x40 the empty
// string (creating the pool first when there is none). Names and the layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkFXLinkEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    CStrId field_0x10;
    u8 unk_0x18[0x8];
    UnkStringData* field_0x20;
    u8 unk_0x28[0x18];
    UnkStringData* field_0x40;
};

void fn_7100067ea4(void* self);  // the base constructor
extern void* g_unk_7101aa0f58[];

void fn_7100bb2894(UnkFXLinkEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa0f58 + 2;
    new (&self->field_0x10) CStrId("", true);
    self->field_0x20 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x20 = &g_unk_7101d47108->empty;
    self->field_0x40 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x40 = &g_unk_7101d47108->empty;
}
