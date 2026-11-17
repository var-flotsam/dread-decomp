// CCharClassGroundShockerAIComponent unit. fn_71005a3b98: the check for a hit; 4 when the hitting
// object's id (+0x20) is the id table's +0xec8 one or its collision id (fn_7100864018) is one of three
// ids, else 1. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/CCollisionComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726e8618[3];

struct UnkHitObject_l5a3b98 {
    u8 pad_0x0[0x20];
    CStrId id;  // 0x20
};

struct UnkHitInfo_l5a3b98 {
    u8 pad_0x0[0x60];
    UnkHitObject_l5a3b98* object;  // 0x60
};

struct UnkActor_l5a3b98 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

u32 fn_71005a3b98(void* self, UnkActor_l5a3b98* actor, void* arg, UnkHitInfo_l5a3b98* info) {
    UnkHitObject_l5a3b98* object = info ? info->object : nullptr;
    const CStrId* collisionId = &g_unk_7101d470e0;
    if (object && actor->collision)
        collisionId = fn_7100864018(actor->collision, object);
    const CStrId* objectId = object ? &object->id : &g_unk_7101d470e0;
    if (*objectId == GameManagerIdTable().field_0xec8 || *collisionId == g_unk_71726e8618[0] ||
        *collisionId == g_unk_71726e8618[1] || *collisionId == g_unk_71726e8618[2])
        return 4;
    return 1;
}
