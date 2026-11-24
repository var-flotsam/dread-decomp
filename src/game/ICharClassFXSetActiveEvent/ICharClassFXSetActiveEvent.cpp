// A constructor in the ICharClassFXSetActiveEvent unit (copy of fn_7100a377a4): the base constructor
// fn_7100067ea4, the vtable, an empty string id at +0x10, then the flag at +0x18 set (+0x19 and +0x1a
// cleared). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkFXSetActiveEventDef {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkStrId field_0x10;
    bool bActive;
    bool bDisableOnEmpty;
    bool bDeleteOnEmpty;
};

extern char g_unk_7101aa0988[];

void fn_7100becf6c(UnkFXSetActiveEventDef* self) {
    fn_7100067ea4(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa0988 + 0x10);
    new (&self->field_0x10) UnkStrId("", true);
    self->bActive = true;
    self->bDisableOnEmpty = false;
    self->bDeleteOnEmpty = false;
}
