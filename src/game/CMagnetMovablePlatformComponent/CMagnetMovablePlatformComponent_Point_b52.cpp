// CMagnetMovablePlatformComponent unit (unit 1407 of tools/tu_map.py): the point at a time along the
// platform's path, fn_710091ad8c. The path (+0x40 of what fn_7100382150 finds for the actor at
// +0x30) is walked to the eased time's share of its length (fn_7100076850, fn_7101059514,
// fn_7101059794), the point of that segment taken (fn_71010596ac) and, when fn_7100deabe4 says so,
// moved by the actor's position. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/SLogicPathNode.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector3D;

struct UnkDashPath;
// A point of the path, handed back in memory (as src/game/CMinimapData.cpp declares it).
struct UnkMinimapPathPoint {
    CVector3D v;
    ~UnkMinimapPathPoint() {}
};

struct UnkMagnetPathHolder_b52 {
    u8 unk_0x0[0x40];
    UnkDashPath* path;
};
UnkMagnetPathHolder_b52* fn_7100382150(UnkActor* actor);
f32 fn_7100076850(f32 t);

struct UnkMagnetMovablePlatform_b52 {
    u8 unk_0x0[0x30];
    UnkActor* actor;
};

UnkMinimapPathPoint fn_710091ad8c(UnkMagnetMovablePlatform_b52* self, f32 t) {
    UnkMagnetPathHolder_b52* holder = fn_7100382150(self->actor);
    f32 eased = fn_7100076850(t);
    UnkDashPath* path = holder->path;
    u32 index = 0;
    f32 fraction = 0.0f;
    fn_7101059794(eased * fn_7101059514(path), path, &index, &fraction);
    UnkMinimapPathPoint point = fn_71010596ac(path, index, fraction);
    if (fn_7100deabe4(g_unk_7101d47310)) {
        UnkActor* actor = self->actor;
        f32 x = actor->field_0x44, y = actor->field_0x54, z = actor->field_0x64;
        point.v.x = x + point.v.x;
        point.v.y = y + point.v.y;
        point.v.z = z + point.v.z;
    }
    return point;
}
