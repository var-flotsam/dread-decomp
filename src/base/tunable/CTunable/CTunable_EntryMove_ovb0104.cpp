// base::tunable::CTunable's unit (unit 111): the move assignment of a registry entry (three string
// ids taken over, the string assigned). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A tunable registry entry (0x160 bytes, as src/base/tunable/CTunable/fn_7100134020.cpp lays it
// out): three string ids and a string with its own storage.
struct UnkTunableEntryMove {
    CStrId field_0x0;
    CStrId name;
    CStrId field_0x10;
    base::global::CRntString field_0x18;
};

UnkTunableEntryMove* fn_7100133054(UnkTunableEntryMove* self, UnkTunableEntryMove* other) {
    self->field_0x0.ReleaseAndTake(other->field_0x0);
    self->name.ReleaseAndTake(other->name);
    self->field_0x10.ReleaseAndTake(other->field_0x10);
    const base::global::CRntString* text = &other->field_0x18;
    fn_710007eaa0(&self->field_0x18, text);
    return self;
}
