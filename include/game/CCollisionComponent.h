#pragma once

// CCollisionComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1319,
// 0x710085fc40-0x7100866e84). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkVector.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkCollisionFileReader;
struct UnkColliderNode;
struct UnkManager_7100863a8c;
struct UnkCollisionContainer;
struct UnkCollisionObject;
struct UnkPlayerMovementSource;
struct UnkStruct_7100861148;
void* fn_710086406c(UnkPlayerMovementSource*, const void*);
void fn_7100861148(UnkStruct_7100861148* self);
void fn_71008618cc(void* self);
void fn_71008656d8(void* object, u32 a, u32 b, void* arg);
extern void* g_unk_7101a79c50[];
void fn_71008615b4(UnkCollisionContainer* self);
void fn_71008659c4(UnkCollisionContainer*, u32, u32, void*);
void fn_7100863e04(UnkCollisionContainer* self, UnkCollisionObject* object);
base::global::CStrId* fn_7100864018(UnkCollisionContainer* self, void* object);
UnkColliderNode* fn_71008625a0(UnkManager_7100863a8c* self, void* owner, const base::global::CStrId* name);
void fn_7100860830(UnkCollisionFileReader* self, const char* path, base::UnkVector<UnkCollisionRecord>* out, bool a, void* b);
