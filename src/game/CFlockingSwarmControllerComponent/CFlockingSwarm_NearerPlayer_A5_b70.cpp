// CFlockingSwarmControllerComponent unit (1158): sort comparison "a is nearer the player than b" (squared
// distances from the player's translation, positions from fn_7100745e24). The terms follow fn_710072a110's
// comparison (src/game/CBillboardComponent/fn_710072a110.cpp). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkObj_710072a110;
struct UnkParent_71007455dc;
UnkVec3Result fn_7100745e24(UnkParent_71007455dc* parent);

struct UnkPlayer;

bool fn_7100739e4c(UnkObj_710072a110* a, UnkObj_710072a110* b) {
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    f32 x = player->field_0x44;
    f32 y = player->field_0x54;
    f32 z = player->field_0x64;
    UnkVec3Result pa = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(a));
    f32 da = (pa.x - x) * (pa.x - x) + (pa.y - y) * (pa.y - y) + (pa.z - z) * (pa.z - z);
    UnkVec3Result pb = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(b));
    f32 db = (pb.x - x) * (pb.x - x) + (pb.y - y) * (pb.y - y) + (pb.z - z) * (pb.z - z);
    return da < db;
}
