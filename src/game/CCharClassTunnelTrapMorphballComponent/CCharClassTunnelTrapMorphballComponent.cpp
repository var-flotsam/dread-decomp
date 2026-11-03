// In the CCharClassTunnelTrapMorphballComponent unit: fn_7100a437d8 hands the member at +0x70 a counted copy of the owner's (+0x18)
// name id at +0x30 (fn_7100a4386c), a copy of fn_710090f4a8 with another member offset (CLogicActionTriggerComponent.cpp).
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "types.h"

using base::UnkStrIdMember;

void fn_7100a4386c(char* member, base::UnkStrIdMember* id);

void fn_7100a437d8(char* self) {
    char* m = self + 0x70;
    base::global::UnkStringData* d =
        *reinterpret_cast<base::global::UnkStringData* const*>(*reinterpret_cast<char**>(self + 0x18) + 0x30);
    union Id {
        base::UnkStrIdMember id;
        Id() {}
        ~Id() { id.~UnkStrIdMember(); }
    } u;
    u.id.data = d;
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
    fn_7100a4386c(m, &u.id);
}
