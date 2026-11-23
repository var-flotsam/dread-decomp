// Sets a string id to the one built from the string fn_7100ad3614 gives for `source`. Names are
// placeholders.
#include "base/UnkStrIdSlot.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1632.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

CStrId* fn_7100ad37c0(CStrId* self, void* source) {
    UnkStrIdSlot slot;
    CRntString str = fn_7100ad3614(source);
    CStrId_ConstructFromRntString(&slot.id, str, true);
    self->ReleaseAndTake(slot.id);
    return self;
}
