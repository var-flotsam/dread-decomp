// CAutsharpAIComponent's unit (tools/tu_map.py unit 572), a copy of fn_71005d2334: whether the owner's movement (+0x218)
// is not on a navigation-mesh side whose neighbouring cell is `cell`: true without movement, without a
// valid hit (virtual 0x2d8) or without a mesh. UnkNavMeshHit as in src/game/navmesh/CNavMesh.cpp.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavMeshHit;
struct UnkElem_7100286624;  // the same 12-byte record (src/game_lo/fn_7100286624.cpp)
void fn_7100287598(UnkNavMeshHit* self);
void fn_710028760c(UnkElem_7100286624* self);

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    CVector2D point;

    UnkNavMeshHit() { fn_7100287598(this); }
    UnkNavMeshHit(const UnkNavMeshHit& other) {
        __builtin_memcpy(static_cast<void*>(&cell), &other.cell, 4);  // the cell, the side and the pad
        point.x = other.point.x;
        point.y = other.point.y;
    }
    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }

    bool IsValid() const { return cell != 0xffff && side != 0xff; }
};
static_assert(sizeof(UnkNavMeshHit) == 0xc, "UnkNavMeshHit layout");

struct UnkMovementNav_A2_b71 {
    void** vtable;
    UnkNavMeshHit vf_0x2d8() {
        return reinterpret_cast<UnkNavMeshHit (*)(UnkMovementNav_A2_b71*)>(vtable[0x2d8 / 8])(this);
    }
};

struct UnkAutsharpAI_A2_b71 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

bool fn_71003cc96c(UnkAutsharpAI_A2_b71* self, u16 cell) {
    auto* movement = static_cast<UnkMovementNav_A2_b71*>(self->owner->field_0x218);
    if (!movement)
        return true;
    UnkNavMeshHit hit = movement->vf_0x2d8();
    bool result;
    u16 hit_cell = hit.cell;
    if (hit_cell != 0xffff && hit.side != 0xff) {
        u8 side = hit.side;
        UnkNavMeshManager* manager = g_unk_71726a4510;
        if (!manager) {
            manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(sizeof(UnkNavMeshManager), 0x3f));
            fn_7100ebf278(manager);
            g_unk_71726a4510 = manager;
        }
        if (manager->navmesh && fn_71002afbe4(fn_710027a328(manager->navmesh, hit_cell), &side)->field_0x2 == cell) {
            result = false;
            goto done;
        }
    }
    result = true;
done:
    return result;
}
