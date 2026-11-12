// CCharClassLavaPoolComponent unit (tools/tu_map.py, the name from reflection): after the base step
// (fn_7100901144), clears the word at +0xbc0, copies the five values the class gave (+0xc90, see
// CCharClassLavaPoolComponent_Init_b50.cpp) to +0xbc4, and resets the damage request at +0xac0
// (fn_71008c1538) with 0. Names and layouts are placeholders.
#include "game/UnkTU1370.h"
#include "types.h"

struct UnkRequest;
void fn_7100901144(char* self);

void fn_71008fc678(char* self) {
    fn_7100901144(self);
    const u32* values = reinterpret_cast<const u32*>(self + 0xc90);
    u32* out = reinterpret_cast<u32*>(self + 0xbc0);
    out[0] = 0;
    out[1] = values[0];
    out[2] = values[1];
    out[3] = values[2];
    out[4] = values[3];
    out[5] = values[4];
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self + 0xac0), 0.0f);
}
