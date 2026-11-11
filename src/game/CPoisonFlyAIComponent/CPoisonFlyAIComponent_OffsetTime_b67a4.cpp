// CPoisonFlyAIComponent's unit (tools/tu_map.py, the name from reflection): an update step. Asks the
// registry at +0x80 of the manager g_unk_71726af468 (created on first use) for an attack offset for
// the owner (+0x30) with flag 1, a zeroed position and a scratch object, as
// CPoisonFlyRequestAttackOffset_Update_b65a4.cpp does; when one was found and the scratch's byte at +8
// is set, the time at +0x14774 restarts, else it grows by `dt`. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU958.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

// As src/game/behaviortree/CPoisonFlyRequestAttackOffset/CPoisonFlyRequestAttackOffset_Update_b65a4.cpp
// declares them.
struct UnkObject_71002ad0dc;

struct UnkOffsetScratch_b65a4 {
    u8 data[0x12a88];
    UnkOffsetScratch_b65a4() { fn_71002acfe4(this); }
    ~UnkOffsetScratch_b65a4() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};

struct UnkOffsetVec_b65a4 {
    f32 x, y, z;
};

struct UnkPoisonFlyAI_b67a4 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x14774 - 0x38];
    f32 time;  // 0x14774
};

void fn_71006393d4(UnkPoisonFlyAI_b67a4* self, f32 dt) {
    UnkOffsetVec_b65a4 pos;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    UnkOffsetScratch_b65a4 scratch;
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 3));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    bool found = fn_710063b1b0(manager->pad_0x0 + 0x80, self->owner, 1, &pos, &scratch);
    // The original computes the time's address on each path (pinned with asm).
    f32* time;
    if (found) {
        bool restart = scratch.data[8];
        time = &self->time;
        asm("" : "+r"(time));
        if (restart) {
            *time = 0.0f;
            return;
        }
    } else {
        time = &self->time;
        asm("" : "+r"(time));
    }
    *time += dt;
}
