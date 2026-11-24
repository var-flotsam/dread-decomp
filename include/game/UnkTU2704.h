#pragma once

// UnkTU2704: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2704,
// 0x7101120afc-0x71011336b0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/UnkTUProperty.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkSpawnHolder;
struct UnkAIGroup_g;
// The AI group of an actor's AI source.
UnkAIGroup_g* fn_71011277a4(void* src);
struct UnkStruct_7172a0768c;
struct UnkObj_7101129240;
void fn_71011258e4(void* self);
void fn_7101129b08(UnkObj_7101129240* self, bool flag);
void fn_7101125c90(char*);
struct CSpawnPointComponent;
// Enable and disable the component and its entity; when automanaged, the "Enabled" property of the current
// scenario's blackboard section follows. Both return true.
bool fn_71011269c0(CSpawnPointComponent* self);
bool fn_7101126af4(CSpawnPointComponent* self);
// The component's "Enabled" property.
extern UnkProperty g_unk_7172a06908;
void fn_710112e7e0(void* self);  // Re-points `current` at (index, element).
extern UnkStruct_7172a0768c g_unk_7172a0768c;  // The id list's count, its ids after it.
const base::global::CStrId* CSpawnPointComponent_GetField0x60TargetId(void* link);
base::reflection::UnkReflObject* fn_7101126814(const base::global::CStrId* id);
void fn_7101131b98(base::global::CRntVector<UnkSpawnHolder>* self, const UnkSpawnHolder* value);
extern void* g_unk_7101cb2880[];
struct CSpawnerActorBlueprint;
extern void* g_unk_7101cb28c8[];  // CSpawnPointComponent's vtable (its destructor fn_7101125c90 stores it).
// Drop the collider made for the scene shape (fn_71011264b4) or the collision scene shape (fn_71011265c0)
// and what that shape is tracked as.
void fn_71011264b4(CSpawnPointComponent* self);
void fn_71011265c0(CSpawnPointComponent* self);
// Make them again for `shape`: the scene shape's (fn_7101128e88), the collision scene shape's (fn_7101129064).
void fn_7101128e88(CSpawnPointComponent* self, void* shape);
void fn_7101129064(CSpawnPointComponent* self, void* shape);
void fn_71011266cc(base::global::CRntVector<CSpawnerActorBlueprint>* self);  // Frees the vector.
// Destroys the last actor blueprint; false when there is none.
bool fn_710112c3dc(CSpawnPointComponent* self);
struct UnkWeakTarget;
// Records a spawn request: the first target's weak handle (+0x348), the flags at +0x330..+0x338 and the last
// target's weak handle (+0x340).
void fn_7101127c24(CSpawnPointComponent* self, UnkWeakTarget* first, bool a, bool b, u32 value, bool c,
                   UnkWeakTarget* second);
// Makes the colliders again for the two shape links (wpSceneShapeId, wpCollisionSceneShapeId) when set.
void fn_710112b7c4(CSpawnPointComponent* self);
