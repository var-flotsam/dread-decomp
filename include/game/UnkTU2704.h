#pragma once

// UnkTU2704: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2704,
// 0x7101120afc-0x71011336b0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/UnkTUProperty.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkEntry_b42a3;
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
const base::global::CStrId* fn_710112bf20(void* link);
base::reflection::UnkReflObject* fn_7101126814(const base::global::CStrId* id);
void fn_7101131b98(base::UnkVector<UnkEntry_b42a3>* self, const UnkEntry_b42a3* value);
extern void* g_unk_7101cb2880[];
