// GUI::IDisplayObjectTrack unit: clearing an array (base::global::CRntVector) of 0x38-byte keys, each a word
// followed by a string (base::global::CRntString) and destroyed by the string's destructor.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkTrackKey {
    u64 field_0x0;
    alignas(8) u8 name[sizeof(base::global::CRntString)];  // a base::global::CRntString

    ~UnkTrackKey() { CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(name)); }
};
static_assert(sizeof(UnkTrackKey) == 0x38, "UnkTrackKey layout");

void fn_7100b1afb8(base::global::CRntVector<UnkTrackKey>* self) {
    self->Clear();
}
