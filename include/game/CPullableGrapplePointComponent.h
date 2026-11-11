#pragma once

// CPullableGrapplePointComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1295,
// 0x710082c6a8-0x710082d53c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkDoorGrapplePoint_A5_b66;
struct UnkPullableGrapplePoint_b54;
bool fn_710082cb24(void* self, void* arg, void* msg);  // The base test (both pull components share it).
void fn_710082cee4(UnkPullableGrapplePoint_b54* self, bool);
void fn_710082cf08(UnkPullableGrapplePoint_b54* self, bool);
void fn_710082ccec(UnkDoorGrapplePoint_A5_b66* self, void*, bool);
