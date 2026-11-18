// Unit 2702 (hint CAIComponent): the base side check: whether the AI may cross side `side` of navmesh
// cell `id`: refused when the blocked-sides map (`blocked`) marks it, unless the AI is in its
// chase state with a target, and refused when the navmesh's own test (fn_71002b106c) fails. Names and
// layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkFlagRows;
bool fn_71002882d0(UnkFlagRows* self, u16 row, u8 column);
bool fn_71002b106c(UnkNavMesh* mesh, u16 id, u8 side, u32 a3, u32 a4, u32* out);

struct UnkAISettings_l10f3cc {
    u8 pad_0x0[0x48a];
    bool ignoreState;  // 0x48a
};

struct UnkAIState_l10f3cc {
    u8 field_0x0;
    u8 field_0x1;
    u8 field_0x2;
    u8 field_0x3;

    // Whether the blocked sides apply in this state.
    bool ChecksBlocked() const {
        return field_0x0 == 0 || field_0x1 == 0 || field_0x2 != 0 || field_0x3 != 0;
    }
};

struct UnkAI_l10f3cc {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x15d8];
    UnkAISettings_l10f3cc* settings;  // 0x1610
    u8 pad_0x1618[0x13090];
    UnkAIState_l10f3cc state;  // 0x146a8
};
static_assert(__builtin_offsetof(UnkAI_l10f3cc, state) == 0x146a8, "layout");

static inline bool ChecksBlocked(UnkAI_l10f3cc* ai) {
    return ai->settings->ignoreState || !fn_7100d7c990(ai->actor) || ai->state.ChecksBlocked();
}

bool fn_710110f3cc(UnkChozoRobotSoldierAI_b66a2* self, u16 id, u32 a2, void* a3, void* a4, void* a5, u32 a6, u32 a7) {
    UnkAI_l10f3cc* ai = reinterpret_cast<UnkAI_l10f3cc*>(self);
    bool checkBlocked = ChecksBlocked(ai);
    if (a5 && checkBlocked && fn_71002882d0(static_cast<UnkFlagRows*>(a5), id, a2))
        return false;
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(sizeof(UnkNavMeshManager), 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    if (manager->navmesh) {
        u32 out = 0;
        if (!fn_71002b106c(manager->navmesh, id, a2, a6, a7, &out))
            return false;
    }
    return true;
}
