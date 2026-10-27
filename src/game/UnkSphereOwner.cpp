// An object of unit 251 (tools/tu_map.py, initialiser fn_71001bde20) with a sphere at +0x20
// (game/UnkSphere.h; its destructor is src/game_s1/fn_71001bd92c.cpp) that follows the object at
// +0x78. Holds fn_71001bdc3c.
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSphereSource;

// The source's centre and radius (the radius stays -1 when it has none).
void fn_710019fc40(UnkSphereSource* self, UnkVec3f* center, f32* radius);

struct UnkSphereOwner {
    u8 pad_0x0[0x20];
    UnkSphere sphere;
    u8 pad_0x70[0x8];
    UnkSphereSource* source;  // 0x78
};
static_assert(__builtin_offsetof(UnkSphereOwner, source) == 0x78,
              "UnkSphereOwner placeholder layout");

// Moves the sphere to the source's centre and gives it the source's radius.
void fn_71001bdc3c(UnkSphereOwner* self) {
    f32 radius;
    UnkVec3f center(0.0f, 0.0f, 0.0f);
    radius = -1.0f;
    fn_710019fc40(self->source, &center, &radius);
    fn_7100216978(&self->sphere, &center);
    fn_710021693c(&self->sphere, radius);
}
