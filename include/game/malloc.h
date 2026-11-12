#pragma once

// malloc: functions and globals of this translation unit that other files use (tools/tu_map.py unit 110,
// 0x71001259f4-0x7100132c04). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkBox2D;
struct UnkContact2D;
struct UnkContactList2D;
struct UnkOrientedBox2D;
struct UnkSegment2D;
struct UnkVec2Result;
struct UnkCircle2D;
struct UnkContactPoint;
struct UnkStatsObject;
void fn_7100131f48(UnkStatsObject* self);
bool fn_710012f260(const UnkCircle2D* a, void* x5, const void* b, UnkContactPoint* out);
void fn_7100132aa0(bool on);
bool fn_710012dd70(const UnkCircle2D* a, void* x5, const UnkCircle2D& b, UnkContactPoint* out);
extern u64 g_unk_710b371030[2];  // The user id.
extern s32 g_unk_710b371068;  // The operation mode (src/engine/SystemStartup.cpp).
UnkSegment2D* fn_7100125b44(UnkSegment2D* self, const UnkSegment2D* other);

// An aligned box moving by `step` against a segment (the contact's param is where along the step).
bool fn_710012ce70(const UnkBox2D* box, const UnkVec2Result& step, const UnkSegment2D* seg, UnkContact2D* out);

bool fn_7100127bb0(const UnkOrientedBox2D* box, const UnkSegment2D* seg, UnkContactList2D* list, bool inside);
bool fn_710012deb0(const UnkCircle2D* circle, const UnkVec2Result* delta, const UnkBox2D* box, UnkContact2D* out);
bool fn_710012e5c0(const UnkBox2D* box, const UnkVec2Result* delta, const UnkCircle2D* circle, UnkContact2D* out);
void fn_7100132248(UnkStatsObject* self, const base::global::CStrId& name, f32 value);
