// An unidentified polymorphic base (vtable unk_7101cb5030) naming a music entry: its unit
// constructs it, destroys it (fn_71011867d4) and passes the name to the music manager's
// singleton (fn_71011867ec, which then runs an optional callback). Seven classes build on it.
#include "base/global/CRntString.h"
#include "types.h"

extern void* unk_7101cb5030[];  // vtable (reached through the GOT)

void fn_710007e6c4(void* self);  // constructs an empty base::global::CRntString

struct UnkMusicTrigger {
    void** vtable;
    base::global::CRntString name;  // +0x8
    bool field_0x38;
    bool field_0x39;
    bool field_0x3a;
    bool field_0x3b;
    bool field_0x3c;
    void* field_0x40;  // a callback, run when set
    u64 field_0x48;
};

// The constructor: an empty name.
void fn_7101186780(UnkMusicTrigger* self) {
    self->vtable = &unk_7101cb5030[2];
    fn_710007e6c4(&self->name);
    self->field_0x38 = true;
    self->field_0x39 = true;
    self->field_0x3a = false;
    self->field_0x3b = false;
    self->field_0x3c = false;
    self->field_0x40 = nullptr;
    self->field_0x48 = 0;
    fn_710007e77c(&self->name, "");
}
