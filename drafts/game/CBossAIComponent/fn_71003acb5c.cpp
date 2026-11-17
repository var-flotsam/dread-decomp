// CBossAIComponent unit: the boss's per-frame step after the base one (fn_7100369650). Unless the flag
// object at +0x14908 says otherwise, looks up the two landmarks named at +0x146f0 and stores their
// positions, then reads the blackboard flag named after the linked actor into +0x14808.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CBossAIComponent.h"
#include "game/CGameLink.h"
#include "game/CLandmarkComponent.h"
#include "game/CLifeComponent.h"
#include "game/CScenario.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

UnkVec3f fn_71008a32f8(UnkLuaNode_b0160* node);

struct UnkBossNamedDef_l3acb5c : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkBossNamedDef_l3acb5c g_unk_7172759848;

static inline UnkSubId_7100fc4b34 SubId(UnkBossNamedDef_l3acb5c& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

struct UnkBossLinkedActor_l3acb5c {
    u8 pad_0x0[0x30];
    CStrId name;
};

struct UnkBossLinkSource_l3acb5c {
    u8 pad_0x0[0x60];
    CGameLink<UnkBossLinkedActor_l3acb5c> link;

    UnkBossLinkedActor_l3acb5c* Linked() {
        if (link.state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(&link.callbacks);
                fn_7100234f78(&link.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            link.state = 2;
        }
        return link.Target();
    }
};

struct UnkBossChild_l3acb5c {
    u8 pad_0x0[0x10];
    UnkBossLinkSource_l3acb5c* object;
};

struct UnkBossHolder_l3acb5c {
    u8 pad_0x0[0x48];
    UnkBossChild_l3acb5c* child;
};

struct UnkBossOwner_l3acb5c {
    u8 pad_0x0[0x1b8];
    UnkBossHolder_l3acb5c* holder;
};

struct UnkBossFlags_l3acb5c {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
};

struct UnkBossLandmarkPoint_l3acb5c {
    bool found;
    UnkVec3f position;
};

struct UnkBossLandmarks_l3acb5c {
    CStrId ids[2];
    u8 pad_0x10[0x4];
    UnkBossLandmarkPoint_l3acb5c points[2];
};

struct UnkBossAI_l3acb5c {
    u8 pad_0x0[0x30];
    UnkBossOwner_l3acb5c* owner;
    u8 pad_0x38[0x146f0 - 0x38];
    UnkBossLandmarks_l3acb5c landmarks;  // 0x146f0
    u8 pad_0x14728[0x14808 - 0x14728];
    bool field_0x14808;
    u8 pad_0x14809[0x14908 - 0x14809];
    UnkBossFlags_l3acb5c* flags;  // 0x14908
};
static_assert(__builtin_offsetof(UnkBossAI_l3acb5c, landmarks.points[1]) == 0x14714, "layout");
static_assert(__builtin_offsetof(UnkBossAI_l3acb5c, flags) == 0x14908, "layout");

// Stores the position of the named landmark when it exists.
static inline void FindLandmark(const CStrId* id, bool* found, UnkVec3f* position) {
    if (id->data == &g_unk_7101d47108->empty)
        return;
    if (UnkLuaNode_b0160* node = fn_71008a3288(id)) {
        *found = true;
        *position = fn_71008a32f8(node);
    }
}

void fn_71003acb5c(UnkCoreXAI_A2_b73* self_) {
    UnkBossAI_l3acb5c* self = reinterpret_cast<UnkBossAI_l3acb5c*>(self_);
    fn_7100369650(reinterpret_cast<char*>(self));
    if (self->flags->field_0x50c)
        return;
    UnkGameManagerSub_0x608* scenario = g_unk_7101d47310->field_0x608;
    if (scenario) {
        UnkBossLandmarks_l3acb5c* landmarks = &self->landmarks;
        FindLandmark(&landmarks->ids[0], &landmarks->points[0].found, &landmarks->points[0].position);
        FindLandmark(&landmarks->ids[1], &landmarks->points[1].found, &landmarks->points[1].position);
    }
    UnkBossHolder_l3acb5c* holder = self->owner->holder;
    if (!holder)
        return;
    if (!holder->child || !holder->child->object) {
        fn_710115fe20(reinterpret_cast<UnkHolder*>(holder));
        if (!holder->child || !holder->child->object)
            return;
    }
    UnkBossLinkedActor_l3acb5c* actor = holder->child->object->Linked();
    if (!actor)
        return;
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    CStrId section_id = fn_71011a2fbc(scenario);
    UnkSubId_7100fc4b34 key = SubId(g_unk_7172759848, actor->name);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        fn_710006d11c(section, &key.id, &self->field_0x14808);
}
