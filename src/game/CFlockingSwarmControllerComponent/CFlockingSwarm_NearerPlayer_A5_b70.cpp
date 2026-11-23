// CFlockingSwarmControllerComponent unit (1158): sort comparison "a is nearer the player than b" (squared
// distances from the player's translation, positions from fn_7100745e24). The terms follow fn_710072a110's
// comparison (src/game/CBillboardComponent/fn_710072a110.cpp). Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1161.h"
#include "game/UnkTU2553.h"
#include "game/UnkVec3Result.h"
#include "types.h"

struct UnkObj_710072a110;
struct UnkParent_71007455dc;

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
