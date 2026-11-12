#pragma once

// UnkTU2523: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2523,
// 0x7100d70c1c-0x7100d8b794). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkActorPosition;
struct UnkWeakHandle;
struct UnkActor;
struct UnkActor_7101105e44;
struct UnkDropCameraVec3;
struct UnkEntityBounds;
struct UnkEntityBox;
struct UnkEntityWithSound;
struct UnkKraidCreatureVec;
struct UnkPlayer;
struct UnkShipRechargeEntity_b49a4;
struct UnkSoundHandle;
struct UnkStrIdPathPair;
struct UnkWeakTarget;
UnkKraidCreatureVec fn_7100d7be08(void* actor);
void fn_7100d7c510(UnkShipRechargeEntity_b49a4* entity, UnkSoundHandle* handle, f32 fade, f32 delay);
const UnkDropCameraVec3* fn_7100d7c9f4(UnkPlayer* player);
UnkSoundHandle fn_7100d7c45c(UnkEntityWithSound* self, void* a, void* b, void* c);
void fn_7100d7b054(void* target, f32 delay);
void fn_7100d7c548(UnkActor* self, const UnkStrIdPathPair* ids, f32 blend, f32 start);
UnkEntityBox fn_7100d7bc70(const UnkEntityBounds* self);
void fn_7100d7add4(UnkActor_7101105e44* actor, bool value);
void fn_7100d7babc(UnkWeakTarget* entity, bool flag, f32 value);
void fn_7100d7c48c(void* target, void* name, f32 a, f32 b);
void fn_7100d80b84(void* self);  // Re-points `current` at (index, element).
extern void* g_unk_71727b85d0;
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
void fn_7100d7c94c(UnkActor*, const u64*);
bool fn_7100d7bea0(UnkActor* owner, bool flag);
UnkActorPosition fn_7100d7b7a8(UnkActor* actor);
