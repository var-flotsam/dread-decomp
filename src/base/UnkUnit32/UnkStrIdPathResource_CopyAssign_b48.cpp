// From unit 32 of tools/tu_map.py: the copy assignment of the id/path resource record (0x18 bytes)
// by copy and move: a copy of `other` (fn_7100027464) is moved into `self` (fn_7100027118) and
// released (fn_7100026f70). Names and layouts are placeholders.
#include "game/UnkTU32.h"
#include "types.h"

struct UnkStrIdPathResource;
struct UnkIdPair;
void fn_7100026f70(UnkIdPair* pair);

struct UnkStrIdPathRecord_b48 {
    u8 data[0x18];
};

UnkStrIdPathRecord_b48* fn_7100027418(UnkStrIdPathRecord_b48* self, const UnkStrIdPathRecord_b48* other) {
    if (self != other) {
        UnkStrIdPathRecord_b48 tmp;
        fn_7100027464(&tmp, other);
        fn_7100027118(reinterpret_cast<UnkStrIdPathResource*>(self), reinterpret_cast<UnkStrIdPathResource*>(&tmp));
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(&tmp));
    }
    return self;
}
