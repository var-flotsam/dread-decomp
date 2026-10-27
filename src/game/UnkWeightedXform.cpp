// From unit 1141 of tools/tu_map.py (0x71007142e8-0x71007153d4, no reflected class): weighted
// transforms (src/game_lo/fn_7100714808.cpp blends two of them); their matrix is an UnkXform's
// (game/UnkXformNode.h). Names are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

// A vector through a matrix (src/game_s1/fn_7100078c9c.cpp).
UnkVec3f fn_710007a400(const UnkVec3f& v, const UnkMatrix44& m);

// The transform's translation through `m`.
UnkVec3f fn_7100714d78(const UnkXform* self, const UnkMatrix44* m) {
    return fn_710007a400(self->GetTranslation(), *m);
}
