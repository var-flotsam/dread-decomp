// Unit 251 of tools/tu_map.py (as UnkSphereOwner.cpp): replaces the object a sphere owner follows
// (+0x78) with one entered in the registry g_unk_716e34c2e0 for the reflected object given
// (fn_71001bd7bc, kind 0x33, with the owner's +0x80 as its key), then moves the sphere at +0x20 to the
// new source's centre and radius (as fn_71001bdc3c) and sets the float at +0x8c to 1 when the source
// says so (fn_710019fd88). Nothing happens without an object. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "game/UnkSphere.h"
#include "game/UnkTU204.h"
#include "game/UnkTU225.h"
#include "game/UnkTU227.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkKeyOwner;
struct UnkSphereSource;
struct UnkSource_710020c780;  // the owner, as src/game_71001e2acc/fn_710020c780.cpp names it

UnkSphereSource* fn_71001bd7bc(void* registry, void* key, UnkReflObject* obj, u32 kind);
bool fn_710019fd88(UnkSphereSource* self);

struct UnkSphereOwner_b59 {
    u8 pad_0x0[0x20];
    UnkSphere sphere;
    u8 pad_0x70[0x8];
    UnkSphereSource* source;  // 0x78
    u8 key_0x80[0xc];
    f32 field_0x8c;
};
static_assert(__builtin_offsetof(UnkSphereOwner_b59, source) == 0x78, "UnkSphereOwner_b59 placeholder layout");

void fn_71001bdc9c(UnkSource_710020c780* owner, UnkReflObject* obj) {
    UnkSphereOwner_b59* self = reinterpret_cast<UnkSphereOwner_b59*>(owner);
    UnkVec3f center;
    f32 radius;
    if (!obj)
        return;
    if (self->source) {
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(self->source));
        self->source = nullptr;
    }
    self->source = fn_71001bd7bc(g_unk_716e34c2e0, self->key_0x80, obj, 0x33);
    center = UnkVec3f(0.0f, 0.0f, 0.0f);
    radius = -1.0f;
    fn_710019fc40(self->source, &center, &radius);
    fn_7100216978(&self->sphere, &center);
    fn_710021693c(&self->sphere, radius);
    if (fn_710019fd88(self->source))
        self->field_0x8c = 1.0f;
}
