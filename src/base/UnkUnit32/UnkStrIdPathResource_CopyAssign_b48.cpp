// From unit 32 of tools/tu_map.py: the copy assignment of the id/path resource record (0x18 bytes)
// by copy and move: a copy of `other` (fn_7100027464) is moved into `self` (fn_7100027118) and
// released (fn_7100026f70). Names and layouts are placeholders.
#include "base/UnkStrIdPathRecord_b48.h"
#include "game/UnkTU32.h"
#include "types.h"

struct UnkStrIdPathResource;
struct UnkIdPair;

UnkStrIdPathRecord_b48* CopyAssignAssetLink(UnkStrIdPathRecord_b48* self, const UnkStrIdPathRecord_b48* other) {
    if (self != other) {
        UnkStrIdPathRecord_b48 tmp;
        fn_7100027464(&tmp, other);
        fn_7100027118(reinterpret_cast<UnkStrIdPathResource*>(self), reinterpret_cast<UnkStrIdPathResource*>(&tmp));
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(&tmp));
    }
    return self;
}
