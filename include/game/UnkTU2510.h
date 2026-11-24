#pragma once

// UnkTU2510: functions of this translation unit that other files call (tools/tu_map.py unit 2510,
// 0x7100d5761c-0x7100d5d23c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkLavaDropsShape_A2_b71;

namespace base::reflection {
struct UnkReflObject;
}
struct UnkColliderResult;
struct UnkScenarioSub;
struct UnkFloorRef_ovb0048;
struct UnkCollisionObject;
struct UnkTree;
struct UnkBvhCollider;
struct UnkTracker_7100863a8c;
struct UnkBox;
struct UnkColliderNode_A6_b70;
struct UnkMtx44;
struct UnkShape;
struct UnkStruct_7100861148_Collider;
struct UnkTree;
bool QueryCollisionTreeForTransformedShapeOverlaps(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* arg, u32 arg2);
void fn_7100d5ac50(void* owner, UnkStruct_7100861148_Collider* group);
bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2);
void fn_7100d5c888(void* owner, UnkColliderNode_A6_b70* collider, u32 old_mask, u32 new_mask);
bool fn_7100d5a930(UnkTracker_7100863a8c* self, UnkCollisionObject* obj);
// Unregisters an object: detaches it from the tracker, removes its record and hands it back.
void fn_7100d5abfc(void* owner, UnkStruct_7100861148_Collider* collider);
void fn_7100d59dd8(UnkScenarioSub* self);
bool fn_7100d5b714(void* tree, u32 mask, const UnkVec3f* from, const UnkVec3f* to, UnkFloorRef_ovb0048* hit, bool closest, bool flag, f32 radius);
// The vtable of the collision filter a shape tester calls back for each candidate.
extern void* g_unk_7101aa8898[];
// The collision world's dispatchers (src/game/logic/collision/UnkCollisionWorld.cpp): each tests a single
// collider directly, or each enabled collider of a group whose layers match. A shape placed by a transform
// against an object; two objects; the first object swept by `delta` against the second.
UnkColliderResult fn_7100d57d68(void* a, void* b, base::reflection::UnkReflObject* obj, void* c, u32 layers, void* d,
                                u32 e);
UnkColliderResult fn_7100d5761c(base::reflection::UnkReflObject* objA, base::reflection::UnkReflObject* objB,
                                u32 layersA, u32 layersB, void* c, u32 d);
UnkColliderResult fn_7100d5823c(base::reflection::UnkReflObject* objA, base::reflection::UnkReflObject* objB,
                                const base::math::CVector2D* delta, u32 layersA, u32 layersB, void* c, bool ownMotion);
bool fn_7100d57ca0(UnkLavaDropsShape_A2_b71* self, void* body, void* other, void* out, u32 flags);
// Queries the tree for what `shape` touches under `mask` (src/game_hi/fn_7100d5bfbc.cpp).
bool fn_7100d5bfbc(UnkTree* self, u32 mask, UnkBvhCollider* shape, void* arg, u32 arg2);
