// Unit 0x71010bee20 (tools/tu_map.py, no name hint): finds an entry by key through fn_71010c1050 and copies
// its 12-byte value out, returning whether it was found. Names and layouts are placeholders.
#include "types.h"

char* fn_71010c1050(char* p0, u32 p1);

struct UnkTriple_b47a3 {
    u32 v[3];
};

bool fn_71010bee90(char* self, UnkTriple_b47a3* out, u32 key) {
    UnkTriple_b47a3* found = reinterpret_cast<UnkTriple_b47a3*>(fn_71010c1050(self, key));
    if (!found)
        return false;
    out->v[0] = found->v[0];
    out->v[1] = found->v[1];
    out->v[2] = found->v[2];
    return true;
}
