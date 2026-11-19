// Unit 53: projects a point along a direction (given, or the normalised offset between two points) onto a
// plane through a third point; optionally returns the distance and the hit, result is the squared miss.
// Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU53.h"
#include "types.h"

struct UnkPoint3f {
    f32 x, y, z;
};

struct UnkPlane4f {
    f32 x = 0.0f, y = 0.0f, z = 0.0f;
    f32 w;
};

f32 fn_7100110ca8(const UnkPoint3f* point, const UnkPlane4f* plane);

f32 fn_71000764d0(const UnkPoint3f* from, const UnkPoint3f* to, const UnkPoint3f* planePoint, UnkPoint3f* outHit,
                  const UnkPoint3f* givenDir, f32* outDist) {
    UnkPlane4f plane;
    f32 dx, dy, dz;
    if (givenDir) {
        dx = -givenDir->x;
        dy = -givenDir->y;
        dz = -givenDir->z;
    } else {
        f32 ox = from->x - to->x;
        f32 oy = from->y - to->y;
        f32 oz = from->z - to->z;
        f32 inv = 1.0f / (sqrtf(ox * ox + oy * oy + oz * oz) + 0.0001f);
        dx = ox * inv;
        dy = oy * inv;
        dz = oz * inv;
    }
    plane.x = dx;
    plane.y = dy;
    f32 d = -(dx * planePoint->x + dy * planePoint->y + dz * planePoint->z);
    plane.z = dz;
    plane.w = d;
    f32 t = -fn_7100110ca8(from, &plane);
    if (outDist)
        *outDist = t;
    f32 hx = plane.x * t + from->x;
    f32 hy = plane.y * t + from->y;
    f32 hz = plane.z * t + from->z;
    if (outHit) {
        outHit->x = hx;
        outHit->y = hy;
        outHit->z = hz;
    }
    f32 ex = planePoint->x - hx;
    f32 ey = planePoint->y - hy;
    f32 ez = planePoint->z - hz;
    return ex * ex + ey * ey + ez * ez;
}
