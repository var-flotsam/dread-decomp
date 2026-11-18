#pragma once

// CGrabComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2709,
// 0x710113a330-0x710113f978). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkMatrix4_b0233;
struct UnkTeleporterActor_b0233;
struct UnkStatics_710113f730;
struct UnkEnergyWaveCompA_b49;
struct UnkKraidGrab_b57a4;
bool fn_710113d83c(void* p);
void fn_710113f270(UnkEnergyWaveCompA_b49* comp, void* object);
void fn_710113d940(void* grabber, UnkKraidGrab_b57a4* grab, void* owner);
extern void* g_unk_7101cb3230[];
extern UnkStatics_710113f730 g_unk_7172a090e8 __attribute__((visibility("hidden")));

// The registry of grabbing components.
struct UnkGrabListTag_ovb0057;
using UnkGrabList_ovb0057 = base::UnkSList<UnkGrabListTag_ovb0057>;
extern UnkGrabList_ovb0057 g_unk_7172a090b8;
void fn_710113cb74(void* component, UnkTeleporterActor_b0233* actor, void* unused, UnkMatrix4_b0233* matrix);
