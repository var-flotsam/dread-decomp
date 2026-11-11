// CScorpiusAIComponent unit (tools/tu_map.py, the name from reflection). fn_71006984cc updates the
// horizontal range at +0x14cf0 from the first and the last point of the path at +0x68 (12-byte points,
// count at +0x70; the last one is left out while the flag at +0x94 is clear), widened by the tunables
// at +0x50 and +0x4c of g_unk_7172703e88, in the order the owner's transform (+0x30, +0x38) faces (the
// sign of its matrix element 2); always false. fn_7100696c54 hands the owner's collision object
// g_unk_71727041b0 to the sound player (fn_7100ed77b8), sets the weight of the anim tree's layer node
// g_unk_7172704218 to 0 (fn_71002e465c) and clears +0x14cc8. fn_7100696d68 (unless the owner's state
// at +0x1b8 is 2) sets that layer weight to 1, restarts the tree (fn_710117774c), runs fn_710069409c
// with g_unk_7172704228 and hands a kind-5 parameter block carrying the owner's z axis to the owner's
// +0x298 (fn_71006f4b0c). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CScorpiusAIComponent.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "game/animtree/CLayerNode.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkTunable;
extern UnkTunable g_unk_7172703e88;

struct UnkScorpiusTunables_ovb0093 {
    u8 pad_0x0[0x4c];
    f32 field_0x4c;
    f32 field_0x50;
};

struct UnkScorpiusOwner_ovb0093 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
};

struct UnkScorpiusAI_ovb0093 {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner_ovb0093* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkVec3f* points;  // 0x68
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 flag;  // 0x94
    u8 pad_0x95[0x14cf0 - 0x95];
    f32 range[2];  // 0x14cf0
};

static inline const UnkScorpiusTunables_ovb0093* Tunables() {
    return reinterpret_cast<const UnkScorpiusTunables_ovb0093*>(&g_unk_7172703e88);
}

bool fn_71006984cc(UnkScorpiusAI_ovb0093* self) {
    u32 n = self->count;
    u32 last = n - ((self->flag == 0) & (n != 0));
    if (last != 0) {
        f32* out = self->range;
        UnkVec3f end = self->points[last];
        UnkVec3f start = self->points[0];
        if (fn_710002614c(&self->owner->xform).m[2] > 0.0f) {
            out[0] = start.x + Tunables()->field_0x50;
            out[1] = end.x - Tunables()->field_0x4c;
        } else {
            out[0] = end.x - Tunables()->field_0x50;
            out[1] = start.x + Tunables()->field_0x4c;
        }
    }
    return false;
}

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkLayerNode;
extern void* g_unk_71726af0b0;
extern CStrId g_unk_71727041b0;  // the collision object's name
extern __attribute__((visibility("hidden"))) u64 g_unk_7172704218;  // the layer node's key
void fn_7100ed77b8(void* player, UnkCollisionObject* object);

struct UnkScorpiusNodeMap_ovb0093 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88

    UnkReflObject** Find(const u64& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }
};

struct UnkScorpiusTree_ovb0093 {
    u8 pad_0x0[0x40];
    UnkScorpiusNodeMap_ovb0093* map;  // 0x40
};

struct UnkScorpiusState_ovb0093 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkScorpiusOwner3_ovb0093 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x140];
    UnkScorpiusState_ovb0093* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkScorpiusTree_ovb0093* tree;  // 0x220
    u8 pad_0x228[0x70];
    void* field_0x298;
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkScorpiusAI3_ovb0093 {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner3_ovb0093* owner;  // 0x30
    u8 pad_0x38[0x14cc8 - 0x38];
    u32 field_0x14cc8;
};

// The layer node under g_unk_7172704218 in the tree, when it is a CLayerNode.
static inline UnkLayerNode* FindLayer(UnkScorpiusTree_ovb0093* tree) {
    UnkScorpiusNodeMap_ovb0093* map = tree->map;
    UnkReflObject** it = map->Find(g_unk_7172704218);
    UnkReflObject* layer = nullptr;
    if (it != map->values + map->count) {
        if (UnkReflObject* node = *it)
            layer = fn_710009d50c(node->vf_0x0(), fn_71002e43a4()) ? node : nullptr;
    }
    return reinterpret_cast<UnkLayerNode*>(layer);
}

void fn_7100696c54(UnkScorpiusAI3_ovb0093* self) {
    if (UnkCollisionContainer* collision = self->owner->collision) {
        if (UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_71727041b0))
            fn_7100ed77b8(g_unk_71726af0b0, object);
    }
    fn_71002e465c(FindLayer(self->owner->tree), 0.0f);
    self->field_0x14cc8 = 0;
}

// As src/game/CScorpiusAIComponent/CScorpiusAIComponent_Timeline_b59.cpp declares it.
struct UnkScorpiusAI_b59a4;
struct UnkScorpiusKey_b59a4;
extern UnkScorpiusKey_b59a4 g_unk_7172704228 __attribute__((visibility("hidden")));
struct UnkShakernautCtrl_b53;

struct UnkScorpiusWords_ovb0093 {
    u32 x, y, z;
};

void fn_7100696d68(UnkScorpiusAI3_ovb0093* self) {
    fn_7100ed78c0(g_unk_71726af0b0);
    UnkScorpiusOwner3_ovb0093* owner = self->owner;
    if (owner->field_0x1b8->state == 2)
        return;
    UnkScorpiusTree_ovb0093* tree = owner->tree;
    fn_71002e465c(FindLayer(tree), 1.0f);
    fn_710117774c(reinterpret_cast<UnkShakernautCtrl_b53*>(tree));
    fn_710069409c(reinterpret_cast<UnkScorpiusAI_b59a4*>(self), &g_unk_7172704228, true, -1.0f, -1.0f);
    owner = self->owner;
    if (!owner || !owner->field_0x298)
        return;
    void* target = owner->field_0x298;
    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    const u32* m = reinterpret_cast<const u32*>(&fn_710002614c(&owner->xform));
    UnkScorpiusWords_ovb0093 axis = {m[2], m[6], m[10]};
    __builtin_memcpy(params.field_0x8, &axis, sizeof(axis));
    params.field_0x8[12] = 1;
    fn_71006f4b0c(target, &params, 0);
}
