// CTakumakuAIComponent unit (tools/tu_map.py): false unless fn_71010fe668; true when fn_71010fc68c
// gives no distance; otherwise whether that distance is at least the one fn_71006d4680 gives (squared
// compare). Names are placeholders.
#include "types.h"

struct UnkTakumakuAI_b64a4;
struct UnkGoliathAI_b58a3;
bool fn_71010fe668(UnkTakumakuAI_b64a4* self);
// fn_71010fc68c is declared elsewhere as returning void; its result is read here, so this declaration
// names the same symbol (same mangled name) with the bool result.
bool fn_71010fc68c_b64a4(UnkGoliathAI_b58a3* ai, f32* out) asm("_Z13fn_71010fc68cP18UnkGoliathAI_b58a3Pf");
void fn_71006d4680(UnkTakumakuAI_b64a4* self, f32* out);

bool fn_71006d45f0(UnkTakumakuAI_b64a4* self) {
    bool ok = fn_71010fe668(self);
    f32 distance = -1.0f;
    if (!ok)
        return false;
    if (!fn_71010fc68c_b64a4(reinterpret_cast<UnkGoliathAI_b58a3*>(self), &distance))
        return true;
    f32 limit = -1.0f;
    fn_71006d4680(self, &limit);
    return distance * distance >= limit * limit;
}
