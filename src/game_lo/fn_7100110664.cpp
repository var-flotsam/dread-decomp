// Sets an angle (wrapped into (-pi, pi]) and rebuilds the 2x2 rotation for the angle plus a
// stored offset (base::spatial::COBox2D's axes).
#include <math.h>

#include "base/spatial/COBox2D.h"
#include "types.h"

using base::spatial::COBox2D;

f32 fn_710007a1d0(f32 angle);  // wraps an angle into (-pi, pi]

void fn_7100110664(COBox2D* self, f32 angle) {
    self->angle = fn_710007a1d0(angle);
    self->axis_x.x = cosf(self->angle + self->offset);
    self->axis_x.y = sinf(self->angle + self->offset);
    self->axis_y.x = -sinf(self->angle + self->offset);
    self->axis_y.y = cosf(self->angle + self->offset);
}
