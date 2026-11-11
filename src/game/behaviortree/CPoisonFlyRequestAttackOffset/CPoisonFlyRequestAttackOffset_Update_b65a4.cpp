// behaviortree::CPoisonFlyRequestAttackOffset's unit (tools/tu_map.py): asks the registry at +0x80 of
// the manager g_unk_71726af468 (created on first use) for an attack offset for the owner (+0x18),
// with a zeroed position and a 0x12a88-byte scratch object (fn_71002acfe4 / fn_71002ad0dc); returns
// 5 when it found one, else 6. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU958.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

// As src/game/behaviortree/CEmmySearchingAction/fn_710051d360.cpp and
// src/game/CAutsniperShootAttack/CAutsniperShootAttack_Dtor_b49.cpp declare them.
struct UnkObject_71002ad0dc;

struct UnkOffsetScratch_b65a4 {
    u8 data[0x12a88];
    UnkOffsetScratch_b65a4() { fn_71002acfe4(this); }
    ~UnkOffsetScratch_b65a4() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};

struct UnkOffsetVec_b65a4 {
    f32 x, y, z;
};

struct UnkOffsetAction_b65a4 {
    u8 pad_0x0[0x18];
    void* owner;  // 0x18
};

s32 fn_7100635bf8(UnkOffsetAction_b65a4* self) {
    UnkOffsetVec_b65a4 pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    UnkOffsetScratch_b65a4 scratch;
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return fn_710063b1b0(manager->pad_0x0 + 0x80, self->owner, 0, &pos, &scratch) ? 5 : 6;
}
