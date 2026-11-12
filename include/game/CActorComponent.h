#pragma once

// CActorComponent: functions of this translation unit that other files call (tools/tu_map.py unit 342,
// 0x710024e2e8-0x7100252568). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBaseComponent;
struct UnkActivatable_b48;
struct UnkLightingComponent3;
struct UnkSmBase_b60;
struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);  // Enables or disables the component.
bool fn_710024fbc8(void*);
bool fn_710024fb2c(UnkActivatable_b48* self);
void fn_710024f910(UnkLightingComponent3* self);  // Derived classes call it as their base's destructor body.
void fn_710024fb24(void*);
void fn_710024fc08();
void fn_710024facc(UnkSmBase_b60*);
void fn_710024f6d4(UnkBaseComponent* self);
