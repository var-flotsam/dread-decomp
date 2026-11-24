// Initialises an object: runs the base initialisation, clears four words at 0x5c and sets a
// three-bit flag set to "all bits" (the complement of a shared empty set, both lazily created
// statics).
#include "base/UnkInitRecord_0x5c.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "types.h"

void fn_71007142e8(UnkStruct_71007142e8* self) {
    fn_7100025624(self);
    self->mode = 0;
    self->vector.x = 0.0f;
    self->vector.y = 0.0f;
    self->vector.z = 0.0f;
    fn_71007143c8(self, UnkFlagsAll_71007142e8());
}
