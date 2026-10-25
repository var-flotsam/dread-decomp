// A positioned sphere (include/game/UnkSphere.h), translation unit 308 of data/tu_map.csv. The
// unit's overlap test fn_7100216c2c is the frozen Phase 3 sample src/flag_id/fn_7100216c2c.cpp,
// which keeps its own view of the layout.
#include "game/UnkSphere.h"
#include "types.h"

// Stores the position (word by word, as the UnkVec3f copy does) and its quantised copy: each
// component times ten, truncated.
void fn_7100216978(UnkSphere* self, const UnkVec3f* position) {
    self->pos = *position;
    self->cell[0] = (s32)(self->pos.x * 10.0f);
    self->cell[1] = (s32)(self->pos.y * 10.0f);
    self->cell[2] = (s32)(self->pos.z * 10.0f);
}
