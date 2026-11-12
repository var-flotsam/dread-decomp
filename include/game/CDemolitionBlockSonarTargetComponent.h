#pragma once

// CDemolitionBlockSonarTargetComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1520,
// 0x7100a0f41c-0x7100a1057c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkOverlayModel;
struct UnkRenderPool;
struct UnkSceneNode;

extern char g_unk_7101a8b8f0[];
UnkSceneNode* fn_7100a0feac(UnkRenderPool* pool, base::global::CFilePathStrId* path, UnkOverlayModel** out, base::global::CStrId* id);
