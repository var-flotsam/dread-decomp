// GUI::IDisplayObjectTrack unit (tools/tu_map.py unit 1671): frees a vector of 0x38-byte entries
// whose string (base::global::CRntString, at +8) is destroyed out of line (shaped like
// fn_7100c9233c in src/clones/). Names and layouts are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkTrackStringEntry_b46a3;
template <>
struct UnkCloneDtorTraits<UnkTrackStringEntry_b46a3> {
    static void destroy(char* p) { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(p + 8)); }
};

using UnkTrackStringEntries_b46a3 = base::UnkVector<UnkCloneDtorElem<UnkTrackStringEntry_b46a3, 0x38>>;

__attribute__((flatten)) void fn_7100b1b23c(UnkTrackStringEntries_b46a3* self) {
    self->Free();
}
