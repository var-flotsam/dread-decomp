// Unit 811 (hint CEmmyAIComponent): the base side check (fn_710110f3cc) refused when the navmesh cell's
// edge on that side (fn_71002afbe4) points up steeply. Names and layouts are placeholders.
#include "game/UnkTU2702.h"
#include "types.h"
#include "base/UnkVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"

struct UnkChozoRobotSoldierAI_b66a2;

bool fn_71005575c4(UnkChozoRobotSoldierAI_b66a2* self, u16 id, u8 side, void* a3, void* a4, void* a5, u32 a6, u32 a7) {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(sizeof(UnkNavMeshManager), 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    if (manager->navmesh) {
        UnkNavMeshCell* cell = fn_710027a33c(manager->navmesh, id);
        if (fn_71002b005c(cell)) {
            UnkNavMeshEdge* edge = fn_71002afbe4(cell, &side);
            if (edge->field_0x10 * -0.0f - edge->field_0xc > 0.9f)
                return false;
        }
    }
    // the base check takes the side as a byte
    using BaseCheck = bool (*)(UnkChozoRobotSoldierAI_b66a2*, u16, u8, void*, void*, void*, u32, u32);
    return reinterpret_cast<BaseCheck>(fn_710110f3cc)(self, id, side, a3, a4, a5, a6, a7);
}
