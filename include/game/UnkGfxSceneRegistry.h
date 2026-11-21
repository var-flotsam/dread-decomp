#pragma once

// The scene registry: the 0xb8-byte singleton g_unk_716e34c2e0 (created by the graphics set-up in
// src/engine/UnkGraphicsSystem.cpp, constructed by fn_710019dcac of unit 225), where cameras, decals
// and model instances are entered. A pooled chained map keyed by address, then a vector. Names and
// layout are placeholders.

#include "base/global/CRntVector.h"
#include "base/input/UnkRumbleManager.h"
#include "types.h"

// A node of the registry's map (0x28 bytes): key, value, the list links, the bucket chain.
struct UnkGfxCameraNode {
    void* key;
    void* value;
    UnkGfxCameraNode* next;   // 0x10
    UnkGfxCameraNode* prev;   // 0x18
    UnkGfxCameraNode* chain;  // 0x20
};

struct UnkGfxSceneRegistry {
    char* vtable;
    base::input::UnkPooledChainedMap map;  // 0x8, its nodes are UnkGfxCameraNode
    base::global::CRntVector<void*> list;  // 0x90
};
static_assert(__builtin_offsetof(UnkGfxSceneRegistry, list) == 0x90, "UnkGfxSceneRegistry placeholder layout");
static_assert(sizeof(UnkGfxSceneRegistry) == 0xb8, "UnkGfxSceneRegistry placeholder layout");
